#!/usr/bin/env python3
# CallTipGenerator.py - regenerate the calltips.txt
# from the ISHelp/isxfunc.xml file.
# Implemented 2025 by YURSHAT <yurshat@gmail.com>
# Requires Python 3.6 or later
# Usage: python CallTipGenerator.py isxfunc.xml

import xml.etree.ElementTree as ET
import pathlib
import argparse

import FileGenerator

# Function to extract name and prototype from isxfunc.xml and return the content as a string
def generateFunctions(inputPath):
	try:
		# Parse the isxfunc.xml file
		tree = ET.parse(inputPath)
		root = tree.getroot()

		# Find all function names to determine the maximum length
		functionNames = [function.find("name").text for function in root.findall(".//function") if function.find("name") is not None]
		maxNameLength = max((len(name) for name in functionNames), default=0)

		# Build the output content
		outputContent = ["[functions]"]
		for function in root.findall(".//function"):
			name = function.find("name").text if function.find("name") is not None else ""
			prototype = function.find("prototype").text if function.find("prototype") is not None else ""
			if name and prototype:
				# Format the line using the dynamically calculated width
				outputContent.append(f"{name.ljust(maxNameLength)} = {prototype}")

		return "\n".join(outputContent)

	except ET.ParseError as e:
		print(f"XML parsing error: {e}")
	except Exception as e:
		print(f"An error occurred: {e}")

if __name__ == "__main__":
	try:
		# Define the root path
		root = pathlib.Path(__file__).resolve().parent.parent
		
		# Set up argument parsing
		parser = argparse.ArgumentParser(description="Parse an ISHelp/isxfunc.xml file and generate calltips content.")
		parser.add_argument("f", type=str, help="Path to the isxfunc.xml file")
		args = parser.parse_args()
		
		f = pathlib.Path(args.f)
		if not f.exists():
			raise FileNotFoundError(f"File '{f}' does not exist.")

		# Generate content and update calltips.txt file
		FileGenerator.UpdateFile(root / "setup" / "calltips.txt", generateFunctions(f))

	except FileNotFoundError as e:
		print(f"Error: {e}")
