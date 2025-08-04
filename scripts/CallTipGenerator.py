#!/usr/bin/env python3
# CallTipGenerator.py - regenerate the calltips.txt
# from the Inno Setup sorce files.
# Implemented 2025 by YURSHAT <yurshat@gmail.com>
# Requires Python 3.6 or later

# Usage: python CallTipGenerator.py C:\issrc

import xml.etree.ElementTree as ET
import pathlib
import argparse
import re

import FileGenerator

def extractKeywordsFromAnsiArray(pasFile):
	content = pasFile.read_text(encoding='mbcs')

	# Remove all real Pascal-style comments, but skip single-quoted strings like '{'
	def stripPascalComments(s):
		# remove { ... } comments, but ignore content in single quotes
		def replacer(match):
			return '' if not re.search(r"'[^']*'", match.group(0)) else match.group(0)
		return re.sub(r'\{[^}]*\}', replacer, s, flags=re.DOTALL)

	content = stripPascalComments(content)
	content = re.sub(r'//.*', '', content)

	pattern = re.compile(
		r'(\w+)\s*:\s*array\s+of\s+AnsiString\s*=\s*\[\s*((?:\s*\'[^\']*\'\s*,?\s*)+?)\s*\];',
		re.MULTILINE
	)

	sets = []
	for match in pattern.finditer(content):
		name = match.group(1)
		raw_items = match.group(2)
		items = re.findall(r"'([^']*)'", raw_items)
		sets.append((name, items))
	return sets

def generateFunctions(xmlPath):
	try:
		tree = ET.parse(xmlPath)
		root = tree.getroot()

		functions = root.findall(".//function")
		pairs = []
		for fn in functions:
			name = fn.find("name").text if fn.find("name") is not None else ""
			proto = fn.find("prototype").text if fn.find("prototype") is not None else ""
			if name and proto:
				pairs.append((name, proto))

		if not pairs:
			return "[functions]"

		pairs.sort(key=lambda x: x[0].lower())  # sort by name, case-insensitive
		maxNameLength = max(len(name) for name, _ in pairs)

		lines = ["[functions]"]
		for name, proto in pairs:
			lines.append(f"{name.ljust(maxNameLength)} = {proto}")

		return "\r\n".join(lines)

	except ET.ParseError as e:
		print(f"XML parsing error in isxfunc.xml: {e}")
	except Exception as e:
		print(f"Error reading isxfunc.xml: {e}")

def generateEvents(pasFile):
	sets = extractKeywordsFromAnsiArray(pasFile)

	events = []

	for name, items in sets:
		if name.lower() != "fulleventfunctions":
			continue

		for item in items:
			item = item.strip()
			match = re.match(r'^(function|procedure)\s+(\w+)', item, re.IGNORECASE)
			if match:
				eventName = match.group(2)
				events.append((eventName, item))

	if not events:
		return "[events]"

	events.sort(key=lambda x: x[0].lower())
	maxNameLength = max(len(name) for name, _ in events)
	lines = ["[events]"]
	for name, full in events:
		lines.append(f"{name.ljust(maxNameLength)} = {full}")

	return "\r\n".join(lines)

def generateConstants(pasFile):
	sets = extractKeywordsFromAnsiArray(pasFile)

	constants = set()  # avoid duplicates

	for name, items in sets:
		if name.lower() != "constants":
			continue
		for item in items:
			if re.fullmatch(r'\w+', item):
				constants.add(item.lower())

	lines = ["[constants]"] + sorted(constants)
	return "\r\n".join(lines)

if __name__ == "__main__":
	try:
		root = pathlib.Path(__file__).resolve().parent.parent

		parser = argparse.ArgumentParser(description="Parse Inno Setup source folder and generate calltips.txt")
		parser.add_argument("dir", type=str, help="Path to Inno Setup source folder (e.g. C:\\issrc)")
		args = parser.parse_args()

		srcDir = pathlib.Path(args.dir)
		isxfunc = srcDir / "ISHelp" / "isxfunc.xml"
		styler = srcDir / "Projects" / "Src" / "IDE.ScintStylerInnoSetup.pas"

		if not isxfunc.exists():
			raise FileNotFoundError(f"Missing file: {isxfunc}")
		if not styler.exists():
			raise FileNotFoundError(f"Missing file: {styler}")

		funcContent = generateFunctions(isxfunc)
		eventsContent = generateEvents(styler)
		constContent = generateConstants(styler)

		# Merge both sections and write to file
		finalContent = "\r\n\r\n".join([funcContent, eventsContent, constContent])
		FileGenerator.Regenerate(root / "setup" / "calltips.txt", "; ", [finalContent])

	except FileNotFoundError as e:
		print(f"Error: {e}")
	except Exception as e:
		print(f"Unhandled exception: {e}")