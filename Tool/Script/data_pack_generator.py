import os
import csv

from logger import init_logger
from config import filter_dict_for_dataclass

class DataPackGenerator:
    def __init__(self, config_cls, **kwargs):
        self.config = config_cls(**filter_dict_for_dataclass(config_cls, kwargs))
        self.logger = init_logger(self.config.log_file_path)
        self.csv_to_cpp_type_map = {
            "int": "int32_t",
            "float": "float",
            "string": "std::string",
            "bool": "bool",
            "int[]": "std::vector<int32_t>",
            "float[]": "std::vector<float>",
            "string[]": "std::vector<std::string>",
            "bool[]": "std::vector<bool>",
        }

    def get_logger(self):
        return self.logger

    def run_generate_header(self):
        csv_path = os.path.join(self.config.target_csv_path, f"{self.config.target_name}.csv")
        if not self.is_exist_csv_file(csv_path):
            return

        with open(csv_path, mode='r', encoding='utf-8', newline='') as f:
            reader = csv.reader(f)
            rows = list(reader)

            if not self.is_valid_csv_file(rows):
                self.logger.error("CSV data is invalid. It needs at least 2 rows: Name, Type, and Data.")
                return

            names_row = rows[0]
            types_row = rows[1]
            if not self.is_valid_schema(names_row, types_row):
                self.logger.error("CSV schema is invalid. Name and Type rows must have the same length.")
                return
            
            data_col_size = len(names_row)
            data_rows = rows[2:]
            if not self.is_valid_data_row(data_rows, data_col_size):
                self.logger.error("CSV data is invalid. Data row must have the same length as Name and Type rows.")
                return

            self.generate_data_pack_header(self.config.target_name, names_row, types_row)
            self.generate_data_pack_parser(self.config.target_name, names_row, types_row)

    def is_exist_csv_file(self, file_path):
        if not os.path.exists(file_path):
            self.logger.error(f"CSV file not found at: {file_path}")
            return False
        self.logger.info(f"Loading CSV file: {file_path}")
        return True

    def is_valid_csv_file(self, rows):
        return len(rows) >= 3

    def is_valid_schema(self, names_row, types_row):
        if len(names_row) != len(types_row):
            return False

        if '' in names_row or '' in types_row:
            return False

        return True

    def is_valid_data_row(self, data_rows, data_col_size):
        data_row_size = len(data_rows)
        if data_row_size <= 0:
            return False

        for data_row in data_rows:
            if len(data_row) != data_col_size:
                return False
        return True

    def generate_data_pack_header(self, target_name, names_row, types_row):
        properties = ""
        msgpack_define = ", ".join(names_row)
        for data_name, data_type in zip(names_row, types_row):
            cpp_data_type = self.csv_to_cpp_type_map.get(data_type)
            if cpp_data_type is None:
                self.logger.error(f"Invalid data type: {data_type}")
                return
            properties += f"    {cpp_data_type} {data_name};\n"
    
        data_pack_header_file = f'''#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <msgpack.hpp>

struct {target_name}DataPack
{{
{properties}
    MSGPACK_DEFINE({msgpack_define});
}};
'''     
        header_file_name = f"{target_name}DataPack.generated.h"
        header_file_path = os.path.join(self.config.output_header_path, header_file_name)
        self.make_data_pack_header_file(header_file_path, data_pack_header_file)

    def generate_data_pack_parser(self, target_name, names_row, types_row):
        parsing_code = ""
        for idx, (data_name, data_type) in enumerate(zip(names_row, types_row)):
            if data_type in ["int", "float", "string"]:
                cpp_type = self.csv_to_cpp_type_map[data_type]
                parsing_code += f"        dataPack.{data_name} = row[{idx}].get<{cpp_type}>();\n"
            elif data_type == "bool":
                parsing_code += f"        dataPack.{data_name} = DataPackUtils::ParseBool(row[{idx}].get<std::string>());\n"
            elif data_type == "int[]":
                parsing_code += f"        dataPack.{data_name} = DataPackUtils::ParseIntArray(row[{idx}].get<std::string>());\n"
            elif data_type == "float[]":
                parsing_code += f"        dataPack.{data_name} = DataPackUtils::ParseFloatArray(row[{idx}].get<std::string>());\n"
            elif data_type == "string[]":
                parsing_code += f"        dataPack.{data_name} = DataPackUtils::ParseStringArray(row[{idx}].get<std::string>());\n"
            elif data_type == "bool[]":
                parsing_code += f"        dataPack.{data_name} = DataPackUtils::ParseBoolArray(row[{idx}].get<std::string>());\n"
        
        template = f'''#pragma once
#include <csv.hpp>

#include "{target_name}DataPack.generated.h"
#include "DataPackUtils.h"

inline std::vector<{target_name}DataPack> Generate{target_name}DataPacks(const std::string& filePath)
{{
    csv::CSVReader reader(filePath);
    std::vector<{target_name}DataPack> dataPacks;
    bool isTypeDefineRow = true;
    for (csv::CSVRow& row : reader)
    {{
        if (isTypeDefineRow)
        {{
            isTypeDefineRow = false;
            continue;
        }}
        {target_name}DataPack dataPack;
{parsing_code}
        dataPacks.push_back(dataPack);
    }}
    return dataPacks;
}}'''

        parser_file_name = f"{target_name}DataPackParser.generated.h"
        parser_file_path = os.path.join(self.config.output_parser_path, parser_file_name)
        self.make_data_pack_header_file(parser_file_path, template)

    def make_data_pack_header_file(self, header_file_path, header_file):
        with open(header_file_path, mode='w', encoding='utf-8') as f:
            f.write(header_file)
        self.logger.info(f"Generated header file: {header_file_path}")