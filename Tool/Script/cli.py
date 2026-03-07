import click

from cmake_executor import CMakeExecutor
from xlsx_to_csv_converter import XLSXToCSVConverter
from data_pack_header_generator import DataPackHeaderGenerator
from config import SolutionConfig, SolutionBuildCofig, BuildConfig, PackageConfig, DataPackConfig, BatchDataPackConfig, ConvertXLSXToCSVConfig, BatchConvertXLSXToCSVConfig

@click.group()
def cli():
    pass

@cli.command()
@click.option("--solution-path", required=True)
@click.option("--log-file-path", required=True)
def generate(**kwargs):
    logger = None
    try:
        cmake_executor = CMakeExecutor(SolutionConfig, **kwargs)
        logger = cmake_executor.get_logger()
        cmake_executor.run_generate()
    except Exception as e:
        if logger:
            logger.error(f"Generate Failed: {e}")
        else:
            print(f"Generate Failed: {e}")

@cli.command()
@click.option("--solution-path", required=True)
@click.option("--config", type=click.Choice(["Debug", "Release", "RelWithDebInfo", "MinSizeRel"]), required=True)
@click.option("--is-rebuild", type=bool, required=True) 
@click.option("--log-file-path", required=True)
def build_solution(**kwargs):
    logger = None
    try:
        cmake_executor = CMakeExecutor(SolutionBuildCofig, **kwargs)
        logger = cmake_executor.get_logger()
        cmake_executor.run_build_solution()
    except Exception as e:
        if logger:
            logger.error(f"Solution Build Failed: {e}")
        else:
            print(f"Solution Build Failed: {e}")

@cli.command()
@click.option("--solution-path", required=True)
@click.option("--config", type=click.Choice(["Debug", "Release", "RelWithDebInfo", "MinSizeRel"]), required=True)
@click.option("--target-name", required=True)
@click.option("--is-rebuild", type=bool, required=True) 
@click.option("--log-file-path", required=True)
def build(**kwargs):
    logger = None
    try:
        cmake_executor = CMakeExecutor(BuildConfig, **kwargs)
        logger = cmake_executor.get_logger()
        cmake_executor.run_build()
    except Exception as e:
        if logger:
            logger.error(f"Build Failed: {e}")
        else:
            print(f"Build Failed: {e}")

@cli.command()
@click.option("--solution-path", required=True)
@click.option("--config", type=click.Choice(["Debug", "Release", "RelWithDebInfo", "MinSizeRel"]), required=True)
@click.option("--log-file-path", required=True)
@click.option("--need-build", type=click.Choice(["true", "false"]), required=True)
def package(**kwargs):
    logger = None
    try:
        cmake_executor = CMakeExecutor(PackageConfig, **kwargs)
        logger = cmake_executor.get_logger()
        cmake_executor.run_package()
    except Exception as e:
        if logger:
            logger.error(f"Build Failed: {e}")
        else:
            print(f"Build Failed: {e}")

@cli.command()
@click.option("--target-csv-path", required=True)
@click.option("--target-name", required=True)
@click.option("--output-header-path", required=True)
@click.option("--output-parser-path", required=True)
@click.option("--log-file-path", required=True)
def generate_data_pack_header(**kwargs):
    logger = None
    try:
        data_pack_header_generator = DataPackHeaderGenerator(DataPackConfig, **kwargs)
        logger = data_pack_header_generator.get_logger()
        data_pack_header_generator.run_generate_header()
    except Exception as e:
        if logger:
            logger.error(f"Generate Data Pack Header Failed: {e}")
        else:
            print(f"Generate Data Pack Header Failed: {e}")

@cli.command()
@click.option("--target-csv-path", required=True)
@click.option("--output-header-path", required=True)
@click.option("--output-parser-path", required=True)
@click.option("--log-file-path", required=True)
def generate_data_pack_headers(**kwargs):
    logger = None
    try:
        data_pack_header_generator = DataPackHeaderGenerator(BatchDataPackConfig, **kwargs)
        logger = data_pack_header_generator.get_logger()
        data_pack_header_generator.run_generate_headers()
    except Exception as e:
        if logger:
            logger.error(f"Generate Data Pack Header Failed: {e}")
        else:
            print(f"Generate Data Pack Header Failed: {e}")

@cli.command()
@click.option("--target-xlsx-path", required=True)
@click.option("--target-name", required=True)
@click.option("--output-csv-path", required=True)
@click.option("--log-file-path", required=True)
def convert_xlsx_to_csv(**kwargs):
    logger = None
    try:
        xlsx_to_csv_converter = XLSXToCSVConverter(ConvertXLSXToCSVConfig, **kwargs)
        logger = xlsx_to_csv_converter.get_logger()
        xlsx_to_csv_converter.run_convert()
    except Exception as e:
        if logger:
            logger.error(f"Convert XLSX to CSV Failed: {e}")
        else:
            print(f"Convert XLSX to CSV Failed: {e}")

@cli.command()
@click.option("--target-xlsx-path", required=True)
@click.option("--output-csv-path", required=True)
@click.option("--log-file-path", required=True)
def convert_xlsxs_to_csvs(**kwargs):
    logger = None
    try:
        xlsx_to_csv_converter = XLSXToCSVConverter(BatchConvertXLSXToCSVConfig, **kwargs)
        logger = xlsx_to_csv_converter.get_logger()
        xlsx_to_csv_converter.run_converts()
    except Exception as e:
        if logger:
            logger.error(f"Batch Convert XLSX to CSV Failed: {e}")
        else:
            print(f"Batch Convert XLSX to CSV Failed: {e}")

if __name__ == "__main__":
    cli()