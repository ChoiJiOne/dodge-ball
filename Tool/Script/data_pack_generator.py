import os
import openpyxl

from logger import init_logger
from config import filter_dict_for_dataclass

class DataPackGenerator:
    def __init__(self, config_cls, **kwargs):
        self.config = config_cls(**filter_dict_for_dataclass(config_cls, kwargs))
        self.logger = init_logger(self.config.log_file_path)

    def get_logger(self):
        return self.logger

    def run_generate(self):
        excel_path = os.path.join(self.config.target_path, f"{self.config.target_name}.xlsx")
        if not self.is_valid_excel_file(excel_path):
            return

        try:
            workbook = openpyxl.load_workbook(excel_path, data_only=True)
            sheet = workbook.active

            rows = list(sheet.iter_rows(values_only=True))
            if len(rows) < 3:
                self.logger.error("Excel data is invalid. It needs at least 3 rows: Name, Type, and Data.")
                return
            
            names_row = rows[0]
            types_row = rows[1]
            data_rows = rows[2:]
            
            # schema = []
            # for name, type_ in zip(names_row, types_row):
            #     if name and type_:
            #         schema.append({"name": str(name).strip(), "type": str(type_).strip()})
            
            # self.logger.info(f"Parsed Schema: {schema}")
            # self.logger.info(f"Loaded {len(data_rows)} data rows.")

        except Exception as e:
            self.logger.error(f"Failed to parse Excel file: {e}")

    def is_valid_excel_file(self, file_path):
        if not os.path.exists(file_path):
            self.logger.error(f"Excel file not found at: {file_path}")
            return False
        self.logger.info(f"Loading Excel file: {file_path}")
        return True
