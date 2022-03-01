import json
import argparse

from pdfscraper.parser_factory import ParserFactory
from pdfscraper.parsers import parser_utils
from pdfscraper.models import errors


def main():
    """Retrieves a PDF file path from the command line arguments and
    extracts sensor specifications from that PDF. The result is output
    to stdout formatted as a JSON string to allow easy transfer to other
    software.
    """
    # Retrieve the path of the pdf from the command line arguments
    command_parser = argparse.ArgumentParser()
    command_parser.add_argument('pdf_path', help='The path to the PDF datasheet to extract information from.')
    args = command_parser.parse_args()

    try:
        data = pdf_scraper(args.pdf_path)
        data['fatal_error'] = False
        data['file_found'] = True
        data['format_parser_detected'] = True
    except FileNotFoundError:
        data = {}
        data['fatal_error'] = False
        data['file_found'] = False
        data['format_parser_detected'] = False
        parser_utils.log_error()
    except errors.NoParserFoundError:
        data = {}
        data['fatal_error'] = False
        data['file_found'] = True
        data['format_parser_detected'] = False
        parser_utils.log_error()
    # An unexpected exception occured
    except Exception:
        data = {}
        data['fatal_error'] = True
        data['file_found'] = False
        data['format_parser_detected'] = False
        parser_utils.log_error()

    # An unexpected error occured
    if data is None:
        data = {}
        data['fatal_error'] = True
        data['file_found'] = False
        data['format_parser_detected'] = False
        parser_utils.log_error()

    # Display the results formatted as JSON to stdout for another program to easily read in
    print(json.dumps(data, indent=4, ensure_ascii=False))


def pdf_scraper(path):
    """Extracts sensor specifications from a PDF at the specified
    file path.

    Args:
        path (str): The file path of the PDF to be processed.

    Returns:
        dict: A dictionary containing all sensor specifications that
        could be extracted. Values that could not be extracted were
        defaulted to the None type.
    """
    parser = ParserFactory(path).get_parser()
    return parser.parse()


if __name__ == "__main__":
    main()
