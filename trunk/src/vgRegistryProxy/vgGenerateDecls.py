
import io
import os
import re
import sys

apis = [
	'RegCloseKey',
	'RegCreateKeyExW',
	'RegOpenKeyExW'
]

apiDecls = []

patterhArgs = re.compile("\s*[_A-Za-z]+(\([^)]+\))?\s+(CONST\s+)?(?P<ArgType>[_A-Za-z]+)\s+(?P<ArgName>[_A-Za-z]+),?")

def generateDecl(header, api):
	print("parsing %s..." % api)

	match = re.search("WINADVAPI\s+LSTATUS\s+APIENTRY\s+%s\s*\(" % api, header)
	if match == None:
		return
	
	params = []
	startPos = match.end(0) + 1
	while True:
		match = patterhArgs.match(header, startPos)
		if match == None:
			break
		params += [[match.group("ArgType"), match.group("ArgName")]]
		startPos = match.end(0) + 1

	paramStr = ""
	paramStrWithoutType = ""
	for i in range(0, len(params)):
		if i > 0: 
			paramStr += ", "
			paramStrWithoutType += ", "
		paramStr += params[i][0] + " " + params[i][1]
		paramStrWithoutType += params[i][1]

	global apiDecls
	apiDecls += [[paramStr, paramStrWithoutType]]
	
def generateRealDecl(output, idx):
	api = apis[idx]
	paramStr = apiDecls[idx][0]
	paramStrWithoutType = apiDecls[idx][1]
	output.write("\tstatic LSTATUS (APIENTRY *Real_%s)(%s);\n" % (api, paramStr))
	
def generateRealImpl(output, idx):
	api = apis[idx]
	paramStr = apiDecls[idx][0]
	paramStrWithoutType = apiDecls[idx][1]
	output.write("LSTATUS (APIENTRY *vgRegistryImpl::Real_%s)(%s) = ::%s;\n" % (api, paramStr, api))

def generateImplDecl(output, idx):
	api = apis[idx]
	paramStr = apiDecls[idx][0]
	paramStrWithoutType = apiDecls[idx][1]
	output.write("\tLSTATUS APIENTRY %s(%s);\n" % (api, paramStr))

def generateMineDecl(output, idx):
	api = apis[idx]
	paramStr = apiDecls[idx][0]
	paramStrWithoutType = apiDecls[idx][1]
	decl = """
	static LSTATUS APIENTRY Mine_%s(%s)
	{
		return vgRegistryImpl::GetInstance().%s(%s);
	}
	"""
	output.write(decl % (api, paramStr, api, paramStrWithoutType))

def main():
	if len(sys.argv) < 2:
		print("Syntax error")
		return
	if not os.path.exists(sys.argv[1]):
		print("Input file not exists")
		return
	headerPath = sys.argv[1]

	headerFile = io.open(headerPath, "rt")
	header = headerFile.read(-1)
	headerFile.close()

	for i in range(0, len(apis)):
		generateDecl(header, apis[i])

	outputFile = io.open("vgRegistryImpl_Decl.inl", "wt")
	outputFile.write("public:\n")
	for i in range(0, len(apis)):
		generateRealDecl(outputFile, i)
	outputFile.write("\n")
	for i in range(0, len(apis)):
		generateImplDecl(outputFile, i)
	outputFile.write("")
	for i in range(0, len(apis)):
		generateMineDecl(outputFile, i)
	outputFile.close()
	
	outputFile = io.open("vgRegistryImpl_Impl.inl", "wt")
	for i in range(0, len(apis)):
		generateRealImpl(outputFile, i)
	outputFile.close()

main()
