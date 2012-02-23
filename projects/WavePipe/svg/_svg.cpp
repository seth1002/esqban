// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>



struct Point
{
	float x;
	short y;
};


#define _PLAIN_SVG_


#ifdef _PLAIN_SVG_

static char *xml1 = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\
<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n\
\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\
<!-- Created with Inkscape (http://www.inkscape.org/) -->\n\
<svg\n\
   xmlns=\"http://www.w3.org/2000/svg\"\n\
   xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n\
   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n\
   xmlns:cc=\"http://web.resource.org/cc/\"\n\
   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n\
   version=\"1.0\"\n\
   x=\"%.5f\"\n\
   y=\"%.5f\"\n\
   width=\"%.5f\"\n\
   height=\"%.5f\"\n\
   id=\"svg1\">\n\
  <defs id=\"defs3\" />\n\
  <metadata id=\"metadata4\">\n\
    <rdf:RDF id=\"RDF5\">\n\
      <cc:Work rdf:about=\"\" id=\"Work6\">\n\
        <dc:format id=\"format7\">image/svg+xml</dc:format>\n\
        <dc:type id=\"type9\"\n\
           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n\
      </cc:Work>\n\
    </rdf:RDF>\n\
  </metadata>\n\
  <path d=\"";

static char *xml2 = "\" id=\"path909\"\n\
        style=\"stroke:#000000;stroke-opacity:1.0000000;\" />\n</svg>\n";

#else

static char *xml1 = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\
<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n\
\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\
<!-- Created with Inkscape (http://www.inkscape.org/) -->\n\
<svg\n\
   id=\"svg1\"\n\
   sodipodi:version=\"0.32\"\n\
   inkscape:version=\"0.39cvs\"\n\
   xmlns=\"http://www.w3.org/2000/svg\"\n\
   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n\
   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n\
   xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n\
   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n\
   xmlns:cc=\"http://web.resource.org/cc/\"\n\
   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n\
   width=\"%.5f\"\n\
   height=\"%.5f\"\n\
   sodipodi:docbase=\"C:\\Documents and Settings\\sqba\\Desktop\"\n\
   sodipodi:docname=\"drawing.svg\">\n\
  <defs\n\
     id=\"defs3\" />\n\
  <sodipodi:namedview\n\
     id=\"base\"\n\
     pagecolor=\"#ffffff\"\n\
     bordercolor=\"#666666\"\n\
     borderopacity=\"1.0\"\n\
     inkscape:pageopacity=\"0.0\"\n\
     inkscape:pageshadow=\"2\"\n\
     inkscape:zoom=\"0.43415836\"\n\
     inkscape:cx=\"305.25953\"\n\
     inkscape:cy=\"417.84947\"\n\
     inkscape:window-width=\"640\"\n\
     inkscape:window-height=\"524\"\n\
     inkscape:window-x=\"66\"\n\
     inkscape:window-y=\"66\"\n />		\
  <metadata\n\
     id=\"metadata4\">\n\
    <rdf:RDF\n\
       id=\"RDF5\">\n\
      <cc:Work\n\
         rdf:about=\"\"\n\
         id=\"Work6\">\n\
        <dc:format\n\
           id=\"format7\">image/svg+xml</dc:format>\n\
        <dc:type\n\
           id=\"type9\"\n\
           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n\
      </cc:Work>\n\
    </rdf:RDF>\n\
  </metadata>\n\
  <path\n\
     style=\"fill:none;fill-opacity:0.75000000;fill-rule:evenodd;stroke:#3929af;stroke-width:1.0000000pt;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1.0000000;\"\n\
     d=\"";

static char *xml2 = "\" id=\"path909\" />\n</svg>\n";

#endif



















int main(int argc, char* argv[])
{
	short buffer[1024];
	int bufflen = sizeof(buffer);
	long i=0;
	int counter=0, interval=512, readBytes=1;
	int height=400, width=65535;
	float x=0.0f, step=0.01f;

	Point lmin={0,0}, lmax={0,0}, lprev={0,0};

	int bitsPerSample = sizeof(short) * 8;
	int maxSample = (int)pow(2, bitsPerSample);// / 2;
	int scale = maxSample / height;

	if( -1 == setmode ( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	if( -1 == setmode ( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	char *filename = (argc==2?argv[1]:TEXT("waveform.svg"));

	FILE *fp = fopen(filename, "w");


	int headerLen;
#ifdef _PLAIN_SVG_
	headerLen = fprintf(fp, xml1, 0.0f, (float)height/2, (float)width, (float)height);
#else
	headerLen = fprintf(fp, xml1, (float)width, (float)height);
#endif


	x = step;
	do {
		readBytes = fread( buffer, 1, bufflen, stdin );

		UINT n = fwrite( buffer, 1, readBytes, stdout );

		int samples = (readBytes*8) / bitsPerSample / 2;

		float mid = height/2.f;

		for(i=0; i<samples; i+=2)
		{
			if(buffer[i] > lmax.y) {
				lmax.x = x;
				lmax.y = buffer[i];
			} else if(buffer[i] < lmin.y) {
				lmin.x = x;
				lmin.y = buffer[i];
			}

			if(counter >= interval)
			{
				Point pt1={0,0}, pt2={0,0};
				if(lmin.x > lmax.x) {
					memcpy(&pt1, &lmax, sizeof(Point));
					memcpy(&pt2, &lmin, sizeof(Point));
				} else {
					memcpy(&pt1, &lmin, sizeof(Point));
					memcpy(&pt2, &lmax, sizeof(Point));
				}

				fprintf(fp, "M %.5f,%.5f ", x-step,(float)(mid + lprev.y/scale));
				fprintf(fp, "L %.5f,%.5f ", x,	(float)(mid + pt1.y/scale));

				fprintf(fp, "M %.5f,%.5f ", x,	(float)(mid + pt1.y/scale));
				fprintf(fp, "L %.5f,%.5f ", x+step,	(float)(mid + pt2.y/scale));

				/*if(x == 600)
				{
					fprintf(fp, "\n");
					fprintf(fp, "x=%f i=%d scale=%d mid=%d\n", x, i, scale, mid);
					fprintf(fp, "lmin.x=%f, lmin.y=%d\n", lmin.x, lmin.y);
					fprintf(fp, "lmax.x=%f, lmax.y=%d\n", lmax.x, lmax.y);
					fprintf(fp, "pt1.x=%f, pt1.y=%d\n", pt1.x, pt1.y);
					fprintf(fp, "pt2.x=%f, pt2.y=%d\n", pt2.x, pt2.y);
					fprintf(fp, "lprev.x=%f, lprev.y=%d\n", lprev.x, lprev.y);
					fprintf(fp, "%f\n", (float)(mid + pt2.y/scale));
					getchar();
					return -1;
				}*/

				memcpy(&lprev, &pt2, sizeof(Point));
				memset(&lprev, 0, sizeof(Point));

				counter = 0;
			} else
				counter++;

			x += step;
		}
	} while(readBytes > 0);
				
	fprintf(fp, "%s", xml2);

	width = (int)x;
	fseek(fp, 0, SEEK_SET);
	int headerLen1;
#ifdef _PLAIN_SVG_
	headerLen1 = fprintf(fp, xml1, 0.0f, (float)height/2, (float)width, (float)height);
#else
	headerLen1 = fprintf(fp, xml1, (float)width, (float)height);
#endif

	if(headerLen > headerLen1)
	{
		for(int n=0; n<headerLen-headerLen1; n++)
			fputc(' ', fp);
	}

	fclose(fp);

	return 0;
}

