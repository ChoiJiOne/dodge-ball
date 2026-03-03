import click

from cmake_executor import CMakeExecutor
from data_pack_generator import DataPackGenerator
from config import SolutionConfig, SolutionBuildCofig, BuildConfig, PackageConfig, DataPackConfig, BatchDataPackConfig

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
        data_pack_generator = DataPackGenerator(DataPackConfig, **kwargs)
        logger = data_pack_generator.get_logger()
        data_pack_generator.run_generate_header()
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
        data_pack_generator = DataPackGenerator(BatchDataPackConfig, **kwargs)
        logger = data_pack_generator.get_logger()
        data_pack_generator.run_generate_headers()
    except Exception as e:
        if logger:
            logger.error(f"Generate Data Pack Header Failed: {e}")
        else:
            print(f"Generate Data Pack Header Failed: {e}")

if __name__ == "__main__":
    cli()