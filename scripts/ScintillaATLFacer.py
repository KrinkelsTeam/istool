#!/usr/bin/env python3
# ScintillaATLFacer.py - regenerate the AtlScintilla.h
# from the Scintilla.iface interface definition file.
# Implemented 2025 by YURSHAT <yurshat@gmail.com>
# Requires Python 3.6 or later

import pathlib

import Face
import FileGenerator

typeAliases = {
	# Convert iface types to C++ types
	# bool and void are OK as is
	"cells": "LPBYTE",
	"colour": "COLORREF",
	"colouralpha": "COLORREF",
	"findtext": "Sci_TextToFind*",
	"findtextfull": "Sci_TextToFindFull*",
	"formatrange": "Sci_RangeToFormat*",
	"formatrangefull": "Sci_RangeToFormatFull*",
	"int": "int",
	"keymod": "int",
	"line": "int",
	"pointer": "void*",
	"position": "int",
	"string": "LPCSTR",
	"stringresult": "LPSTR",
	"textrange": "Sci_TextRange*",
	"textrangefull": "Sci_TextRangeFull*",
}

def ActualTypeName(type):
	if type in typeAliases:
		return typeAliases[type]
	else:
		return type
		
def IsEnumeration(s):
	if s in ["Position", "Line", "Colour", "ColourAlpha"]:
		return False
	if s.endswith("*"):
		return False
	return s[:1].isupper()	 
	
def generateFunctionBody(name, retType, params, command, comments):
	"""
	Generate C++ function code for a given feature.
	"""
	# Generate comments
	commentBlock = "\r\n\t".join([f"/// {line}" for line in comments]) if comments else f"/// {name}: auto-generated function."

	# Generate parameter list
	paramStrings = []
	for paramType, paramName, default in params:
		if paramType and paramName:  # Only include non-empty parameters
			if IsEnumeration(paramType):
				paramType = "int"
			paramString = f"{ActualTypeName(paramType)} {paramName}"
			if default:  # Add default value if available
				paramString += f" = {default}"
			paramStrings.append(paramString)
	paramList = ", ".join(paramStrings)

	# Generate WPARAM and LPARAM values
	w_param = params[0][1] if len(params) > 0 and params[0][1] else "0"
	l_param = params[1][1] if len(params) > 1 and params[1][1] else "0"

	# Determine the return string based on the return type
	retBool = ""
	if retType == "void":
		retTypeString = ""
	elif retType == "bool":
		retTypeString = "return "
		retBool = " != 0"
	else:
		retTypeString = f"return ({ActualTypeName(retType)})"

	# Generate the function code
	return (
		"\r\n\t" + commentBlock + "\r\n" +
		"\t" + ActualTypeName(retType) + " " + name + "(" + paramList + ") {\r\n" +
		"\t\tATLASSERT(::IsWindow(m_hWnd));\r\n" +
		"\t\t" + retTypeString + "::SendMessage(m_hWnd, " + command + ", (WPARAM)" + w_param + ", (LPARAM)" + l_param + ")" + retBool + ";\r\n" +
		"\t}"
)

def generateFunctions(f):
	"""
	Generate all functions from Scintilla.iface.
	"""
	functions = []
	for name, feature in f.features.items():
		if feature["Category"] != "Deprecated":
			if feature["FeatureType"] in ["fun", "get", "set"]:
				retType = feature["ReturnType"]
				if IsEnumeration(retType):
					retType = "int"
				params = [
					(feature["Param1Type"], feature["Param1Name"], feature["Param1Value"]),
					(feature["Param2Type"], feature["Param2Name"], feature["Param2Value"])
				]
				comments = feature.get("Comment", [])
				functions.append(generateFunctionBody(name, retType, params, "SCI_" + name.upper(), comments))

	return functions
	
if __name__ == "__main__":
	root = pathlib.Path(__file__).resolve().parent.parent
	
	# Load the Scintilla.iface definition
	f = Face.Face()
	f.ReadFromFile(root / "includes" / "scintilla" / "include" / "Scintilla.iface")
	
	# Write the generated code to AtlScintilla.h
	FileGenerator.Regenerate(root / "source" / "Scintilla" / "AtlScintilla.h", "/* ", generateFunctions(f))
