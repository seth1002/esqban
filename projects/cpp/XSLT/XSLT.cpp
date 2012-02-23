// XSLT.cpp : Defines the entry point for the console application.
//

#include <stdio.h>

#include <tchar.h>
#include <comdef.h>
#include <stdio.h>

//#import "msxml3.dll"
#import <msxml3.dll> named_guids

//using namespace MSXML;
//using namespace MSXML2;

inline void TESTHR( HRESULT _hr ) { if FAILED(_hr) throw(_hr); }

//SUCCEEDED


/*
int main(int argc, char* argv[])
{
	IXMLDOMDocument2Ptr doc;
	IXMLDOMDocument2Ptr xsl;
	
	// Initialize COM
	if(FAILED(::CoInitialize(NULL))) {
		printf("CoInitialize failed!\n");
		return 0;
	}

	char *strDOM = "Msxml2.DOMDocument.3.0";
	if(FAILED(doc.CreateInstance(strDOM))) {
		printf("Failed to create %s!\n", strDOM);
		return 0;
	}
	if(FAILED(xsl.CreateInstance(strDOM))) {
		printf("Failed to create %s!\n", strDOM);
		return 0;
	}
	//if (FAILED(doc.CreateInstance(MSXML::CLSID_DOMDocument)))
	//{
	//	printf("Failed to create DOMDocument!\n");
	//	return 0;
	//}
	//if (FAILED(xsl.CreateInstance(MSXML::CLSID_DOMDocument)))
	//{
	//	printf("Failed to create DOMDocument!\n");
	//	return 0;
	//}
	
	_variant_t filename(argv[1]);
	_variant_t varOut((bool)TRUE);

	varOut = doc->load(filename);
	if ((bool)varOut == TRUE) {
		_variant_t filename(argv[2]);
		varOut = xsl->load(filename);
		if ((bool)varOut == TRUE) {
			char *out = (argc>=4 ? argv[3] : "out.xml");
			printf(" %s + %s = %s\n", argv[1], argv[2], out);
			try {
				//_bstr_t output = doc->transformNode(xsl);
				_bstr_t output;
				if(SUCCEEDED(doc->transformNode(xsl, output))) {
					FILE *fp = fopen(out, "w");
					fprintf(fp, "%s", (LPCSTR)output);
					fclose(fp);
					printf("Saved output to file: %s\n", out);
					return 1;
				}
			} catch(...) {
				printf("DOM Exception!");
			}
		} else
			printf("Failed to load %s!\n", filename);
	} else
		printf("Failed to load %s!\n", filename);
	return 0;
}

*/











int main(int argc, char* argv[])
{
	MSXML2::IXMLDOMDocument2 *doc, *xsl;

	// Initialize COM
	if(FAILED(::CoInitialize(NULL))) {
		printf("CoInitialize failed!\n");
		return 0;
	}

	if(FAILED(CoCreateInstance(MSXML2::CLSID_DOMDocument,
		NULL,
		CLSCTX_INPROC_SERVER, 
		MSXML2::IID_IXMLDOMDocument2,
		(void**)&doc)))
	{
		printf("CoCreateInstance failed!\n");
		return 0;
	}

	if(FAILED(CoCreateInstance(MSXML2::CLSID_DOMDocument,
		NULL,
		CLSCTX_INPROC_SERVER, 
		MSXML2::IID_IXMLDOMDocument2,
		(void**)&xsl)))
	{
		printf("CoCreateInstance failed!\n");
		return 0;
	}

	
	_variant_t varOut((bool)TRUE);

	_variant_t xmlfilename(argv[1]);
	varOut = doc->load(xmlfilename);
	if ((bool)varOut == FALSE) {
		printf("Failed to load %s!\n", xmlfilename);
		return 0;
	}

	_variant_t xslfilename(argv[2]);
	varOut = xsl->load(xslfilename);
	if ((bool)varOut == FALSE) {
		printf("Failed to load %s!\n", xslfilename);
		return 0;
	}

	char *out = (argc>=4 ? argv[3] : "out.xml");
	printf(" %s + %s = %s\n", argv[1], argv[2], out);
	try {
		_bstr_t output = doc->transformNode(xsl);
		//_bstr_t output;
		//if(SUCCEEDED(doc->transformNode(xsl, output))) {
			FILE *fp = fopen(out, "w");
			fprintf(fp, "%s", (LPCSTR)output);
			fclose(fp);
			printf("Saved output to file: %s\n", out);
			return 1;
		//}
	} catch(...) {
		printf("transformNode threw an exception!");
	}

	return 0;
}




