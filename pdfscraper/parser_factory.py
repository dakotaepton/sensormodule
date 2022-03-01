import pdfquery

from pdfscraper.parsers import *
from pdfscraper.parsers import parser_utils
from pdfscraper.models.manufacturer import Manufacturer
from pdfscraper.models import errors


class ParserFactory:
    """Determines which datasheet parser to use depending on the contents of the pdf.
    """

    def __init__(self, path):
        """Sets the parser field to the correct datasheet parser for the pdf given.

        Args:
            path (str): The path to the pdf file.

        Raises:
            errors.NoParserFoundError: When no datasheet format has been recognised.
        """
        self.pdf = pdfquery.PDFQuery(path, resort=False)
        self.pdf.load()
        self.parser = None

        # ALPHASENSE
        if 'Alphasense' in parser_utils.search_text(self.pdf, 'Alphasense', page_num=None):
            search_term = '% O2'
            if search_term not in parser_utils.search_text(self.pdf, search_term):
                self.parser = AlphasenseParserFormat1(self.pdf)
            else:
                self.parser = AlphasenseParserFormat2(self.pdf)

        # SENSORIC
        elif 'Sensoric' in parser_utils.search_text(self.pdf, 'Sensoric', page_num=None):
            if 'PART NUMBER INFORMATION' in parser_utils.search_text(self.pdf, 'PART NUMBER INFORMATION', page_num=None):
                self.parser = SensoricParserFormat2(self.pdf)
            elif 'Technical Specifications' in parser_utils.search_text(self.pdf, 'Technical Specifications', page_num=None):
                self.parser = SensoricParserFormat1(self.pdf)
            else:
                self.parser = SensoricParserFormat3(self.pdf)

        # CITYTECH
        elif 'citytech' in parser_utils.search_text(self.pdf, 'citytech', page_num=None):
            self.parser = CityTechParserFormat1(self.pdf)

        # MEMBRAPOR
        elif 'membrapor' in parser_utils.search_text(self.pdf, 'membrapor', page_num=None):
            if 'ELECTROCHEMICAL GAS SENSORS' in parser_utils.search_text(self.pdf, 'ELECTROCHEMICAL GAS SENSORS'):
                self.parser = MembraporParserFormat1(self.pdf)
            elif 'SPECIFICATION SHEET' in parser_utils.search_text(self.pdf, 'SPECIFICATION SHEET'):
                self.parser = MembraporParserFormat2(self.pdf)
            else:
                self.parser = MembraporParserFormat3(self.pdf)

        # WINSEN
        elif 'Winsen' in parser_utils.search_text(self.pdf, 'Winsen', page_num=None):
            self.parser = WinsenParserFormat1(self.pdf)

        # No datasheet format was recognised.
        if self.parser is None:
            raise errors.NoParserFoundError

    def get_parser(self):
        return self.parser
