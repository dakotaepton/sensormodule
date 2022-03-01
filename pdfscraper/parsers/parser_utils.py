import sys
import traceback
import itertools
from collections import OrderedDict

import pdfminer
from pdfquery import PDFQuery

U_OHM = '\u2126'
DEFAULT_PADDING = 5.5
TEXT_ELEMENTS = [
    pdfminer.layout.LTTextBox,
    pdfminer.layout.LTTextBoxHorizontal,
    pdfminer.layout.LTTextLine,
    pdfminer.layout.LTTextLineHorizontal
]


class Justification:
    LEFT = 0
    RIGHT = 1


def search_next_column(pdf, search_term, table_column_widths, justification=Justification.LEFT, padding=DEFAULT_PADDING, search_n_rows=1, page_num=0):
    """Searches for text in the next column over in a table in the initial PDF.

    Args:
        pdf (PDFQuery): The PDF to search.
        search_term (str): The text to search for in the first column.
        table_column_widths (tuple of float): The two column widths of a given table.
        padding (float, optional): The amount of extra space around the next column to look for text.
        search_n_rows (int, optional): How many rows to include in the results.
        page_num (int, optional): The page of the PDF to search the text on. Defaults to 0.

    Returns:
        str: The text found in the column next to the search term. Or an empty string if
        an exception is thrown.
    """
    try:
        next_column = get_next_column(pdf, search_term, table_column_widths, justification, padding, search_n_rows, page_num)
        return search_text_in_bbox(pdf, next_column, page_num=page_num)
    except Exception:
        return ''


def get_next_column(pdf, search_term, column_widths, justification, padding, search_n_rows, page_num):
    """Obtain the pixel coordinates of the next column in a PDF table.

    Args:
        pdf (PDFQuery): The PDF to search.
        search_term (str): The text to search the PDF for.
        column_widths (tuple): A tuple of size two with each element containing the width in points (72 per inch)
            for the respective column.
        padding (float): The amount of extra space around the next column to look for text.
        search_n_rows (int, optional): How many rows to include in the results.
        page_num (int, optional): The page of the PDF to search the text on. Defaults to 0.

    Returns:
        str: A string containing the two coordinates need to specify a bounding box.
    """
    element = get_pyquery_from_text(pdf, search_term, page_num=page_num)
    if element is None:
        return

    row_height = (float(element.attr('y1')) - float(element.attr('y0'))) * (search_n_rows - 1)

    if justification == Justification.LEFT:
        x0 = float(element.attr('x0')) + column_widths[0] - padding
        x1 = float(element.attr('x0')) + column_widths[0] + column_widths[1] + padding
    elif justification == Justification.RIGHT:
        x0 = float(element.attr('x1'))
        x1 = float(element.attr('x1')) + column_widths[1] + padding
    y0 = float(element.attr('y0')) - row_height - padding
    y1 = float(element.attr('y1')) + padding

    return '{0}, {1}, {2}, {3}'.format(x0, y0, x1, y1)


def search_text_in_bbox(pdf, bbox, page_num=0):
    """Searches for text contained in a specific bounding bound defined by two corners.

    Args:
        pdf (PDFQuery): The PDF to search.
        bbox (str): The bounding box to search for text within.
        page_num (int, optional): The page of the PDF to search the text on. Defaults to 0.

    Returns:
        str: The text found in the supplied bounding box.
    """
    if page_num is not None:
        element = pdf.pq('LTPage[page_index="{0}"] LTTextLineHorizontal:in_bbox("{1}")'.format(page_num, bbox))
    else:
        element = pdf.pq('LTTextLineHorizontal:in_bbox("{0}")'.format(bbox))
    return element.text()


def search_text(pdf, search_term, page_num=0):
    """Search for a block of text in the PDF containing the search term.

    Args:
        pdf (PDFQuery): The PDF to search.
        search_term (str): The text to search the PDF for.
        page_num (int, optional): The page of the PDF to search the text on. Defaults to 0.

    Returns:
        str: The block of text containing the search term. Or an empty string if
        an exception is thrown.
    """
    try:
        elements = get_pyquery_from_text(pdf, search_term, page_num).items()
        text = [element.text() for element in elements]
        return '\n'.join(text)
    except Exception:
        return ''


def get_pyquery_from_text(pdf, search_term, page_num=0):
    """Retrieves the bounding box for a block of text in the PDF containing the search term.

    Args:
        pdf (PDFQuery): The PDF to search.
        search_term (str): The text to search the PDF for.
        page_num (int, optional): The page of the PDF to search the text on. Defaults to 0.

    Returns:
        PyQuery: The pdf query object containing the search term element. Or None if
        an exception is thrown.
    """
    try:
        if page_num is not None:
            element = pdf.pq('LTPage[page_index="{0}"] LTTextLineHorizontal:contains("{1}")'.format(page_num, search_term))
        else:
            element = pdf.pq('LTTextLineHorizontal:contains("{0}")'.format(search_term))
        return element
    except Exception:
        return None


def extract_tabular_data(pdf, x_start=0, x_end=10000, x_tolerance=10, y_tolerance=None, y_tolerance_up=0, y_tolerance_down=0, char_height=0, page_num=0):
    """Extracts text from a PDF containing tabular-like data. Such that, it finds columns of data separated by a large space.
    Based off of: https://github.com/janedoesrepo/pdfreader

    Args:
        pdf (PDFQuery): The PDF to extract the tabular data from.
        x_start (int, optional): Only consider characters to the right of x-start. Defaults to 0.
        x_end (int, optional): Only consider characters to the left of x-end. Defaults to 10000.
        x_tolerance (int, optional): The maximum amount of space between characters in the x-axis for them to considered as part of the same word. Defaults to 10.
        y_tolerance (int, optional): The maximum amount of space between characters in the y-axis for them to considered as part of the same word. Defaults to None.
        y_tolerance_up (int, optional): The maximum amount of space between characters in the positive y-axis for them to considered as part of the same word. Defaults to 0.
        y_tolerance_down (int, optional): The maximum amount of space between characters in the negative y-axis for them to considered as part of the same word. Defaults to 0.
        char_height (int, optional): The minimum height of characters to include. Defaults to 0.
        page_num (int, optional): The page of the PDF to extract tabular data from. Defaults to 0.

    Returns:
        str[][]: A two dimensional which contains each row of text, and each row of text contains each column.
    """
    row_texts = []
    text_objects = []

    # Retrieve all of the text boxes on a PDF page
    for element in _extract_page_layout(pdf, page_num):
        if isinstance(element, pdfminer.layout.LTTextBoxHorizontal):
            text_objects.append(element)

    # Find all of the characters inside those text boxes
    characters = _extract_characters(text_objects, x_start, x_end, char_height)

    # Combine characters that are closer than the given tolerances and on the same y axis
    row_ys = sorted(list(set(char.bbox[1] for char in characters if char.get_text() != U_OHM)), reverse=True)
    for row_y in row_ys:
        if y_tolerance is not None:
            y_tolerance_up = y_tolerance
            y_tolerance_down = y_tolerance
        # Sort the characters by their position on the x-axis
        row_sorted = sorted([char for char in characters if char.bbox[1] <= row_y + y_tolerance_up and char.bbox[1] >= row_y - y_tolerance_down], key=lambda char: char.bbox[0])

        column_index = 0
        row_text = []
        for char_i, char in enumerate(row_sorted[:-1]):
            # If the characters are part of the same word
            if (row_sorted[char_i + 1].bbox[0] - char.bbox[2]) > x_tolerance:
                column_text = ''.join([char.get_text() for char in row_sorted[column_index:(char_i + 1)]])
                column_index = char_i + 1
                row_text.append(column_text.strip())
            # Last character of the row
            elif char_i == len(row_sorted) - 2:
                column_text = ''.join([char.get_text() for char in row_sorted[column_index:]])
                row_text.append(column_text.strip())
        row_texts.append(row_text)

    # Remove duplicate and empty entries
    return [text for text, _ in itertools.groupby(row_texts) if len(text,) != 0]


def _extract_page_layout(pdf, page_num):
    """Retrieve the specified page object from the PDF.

    Args:
        pdf (PDFQuery): The PDF to extract the page object from.
        page_num (int): The page number of the page object to return.

    Returns:
        LTPage: The page object from the PDF.
    """
    document = pdf.doc

    resource_manager = pdfminer.pdfinterp.PDFResourceManager()
    device = pdfminer.converter.PDFPageAggregator(resource_manager, laparams=pdfminer.layout.LAParams())
    interpeter = pdfminer.pdfinterp.PDFPageInterpreter(resource_manager, device)

    page = list(pdfminer.pdfpage.PDFPage.create_pages(document))[page_num]
    interpeter.process_page(page)
    layout = device.get_result()

    return layout


def _extract_characters(element, x_start, x_end, char_height):
    """Recursively extracts individual characters from text elements.

    Args:
        element (TEXT_ELEMENT): The text element of a PDF page to extract characters from.
        x_start (int, optional): Only consider characters to the right of x-start. Defaults to 0.
        x_end (int, optional): Only consider characters to the left of x-end. Defaults to 10000.
        char_height (int, optional): The minimum height of characters to include. Defaults to 0.

    Returns:
        str[]: A list of all the characters in the given text element.
    """
    if isinstance(element, pdfminer.layout.LTChar):
        return [element] if element.bbox[0] > x_start and element.bbox[0] < x_end and element.height > char_height else []

    if any(isinstance(element, i) for i in TEXT_ELEMENTS):
        return _flatten([_extract_characters(e, x_start, x_end, char_height) for e in element])

    if isinstance(element, list):
        return _flatten([_extract_characters(l, x_start, x_end, char_height) for l in element])

    return []


def _flatten(list2D):
    """Converts a two dimensional list into a one dimensional list.

    Args:
        list2D str[][]: The 2D list to convert.

    Returns:
        str[]: The resuting 1D list.
    """
    return [item for sublist in list2D for item in sublist]


def find_row_with_substring(text_rows, search_term):
    """Retrieves the row of text which has the search term as a substring.

    Args:
        text_rows (str[][]): The list of rows of text to search.
        search_term (str): The text to search for.

    Returns:
        str[]: The row of text containing each column.
    """
    for text_row in text_rows:
        if search_term in ''.join(text_row):
            return text_row


def find_row_with_string(text_rows, search_term):
    """Retrieves the row of text which a column contains exactly the search term.

    Args:
        text_rows (str[][]): The list of rows of text to search.
        search_term (str): The text to search for.

    Returns:
        str[]: The row of text containing each column.
    """
    for text_row in text_rows:
        for text in text_row:
            if text == search_term:
                return text_row


def log_error():
    """Prints the stack trace and message of the last exception to occur to stderr."""
    print('\n' + traceback.format_exc(), file=sys.stderr)


def debug(message):
    """Prints a string to the stderr stream as the result of the program is printed out to stdout.

    Args:
        message (str): The message to display to the console.
    """
    print(message, file=sys.stderr)
