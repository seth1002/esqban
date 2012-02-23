typedef struct tagPETINFO {
	char szKind[10];
	char szBreed[50];
	char szPrice[20];
}PETINFO;

// A PETINFO variable is declared and initialized as follows:
PETINFO rgPetInfo[ ] = {
	{"Dog", "Poodle", "$300.00"},
	{"Cat", "Siamese", "$100.00"},
	{"Fish", "Angel Fish", "$10.00"},
};






// ListView.cpp: implementation of the CListView class.
//
//////////////////////////////////////////////////////////////////////

#include "ListView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListView::CListView(HINSTANCE hInstance, HWND hWndParent)
{
	int index = 0;
	LV_ITEM lvI;

//	HICON hIcon;


	// Create list view
	RECT  rc;
	GetClientRect(hWndParent, &rc);
	m_hwndList = CreateWindow(WC_LISTVIEW,
							NULL, 
							WS_CHILD | WS_VISIBLE | LVS_REPORT, 
							rc.left,
							rc.top,
							rc.right - rc.left,
							rc.bottom - rc.top,
							hWndParent, 
							ID_LIST, 
							//((LPCREATESTRUCT) lParam)->hInstance,
							hInstance,
							NULL);

	
	//ListView_SetImageList(hwndList, m_himlSmall, LVSIL_SMALL);
	//ListView_SetImageList(hwndList, m_himlLarge, LVSIL_NORMAL);



    char szText[256];     // temporary buffer 
    LVCOLUMN lvc; 
    int iCol; 
 
    // Initialize the LVCOLUMN structure.
    // The mask specifies that the format, width, text, and subitem
    // members of the structure are valid. 
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM; 
     
    // Add the columns. 
    for (iCol = 0; iCol < 3; iCol++) 
	{ 
        lvc.iSubItem = iCol;
        lvc.pszText = szText;	
        lvc.cx = 100;           // width of column in pixels
        if ( iCol < 2 ) {
          lvc.fmt = LVCFMT_LEFT;  // left-aligned column
		} else { 
          lvc.fmt = LVCFMT_RIGHT;  // right-aligned column
        }  
        //LoadString(hInst, 0 + iCol, szText, sizeof(szText));
		sprintf(szText, "Column %d", iCol);
        ListView_InsertColumn(m_hwndList, iCol, &lvc);
    } 



	// Fill list view with test items
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	lvI.state = 0;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.iImage = I_IMAGECALLBACK;
	lvI.pszText = LPSTR_TEXTCALLBACK;
	//lvI.pszText = "test";
	for(int i=0; i<3; i++) 
	{
		lvI.iItem = index;

		lvI.lParam = (LPARAM) &rgPetInfo[index];

//		for(lvI.iSubItem=0; lvI.iSubItem<3; lvI.iSubItem++) {
			//hIcon = pShortcut->hIconLarge;
			//if (NULL == hIcon)
			//	hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_DISK));
			//ImageList_AddIcon(m_himlLarge, hIcon);

			//hIcon = pShortcut->hIconSmall;
			//if (NULL == hIcon)
			//	hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_DISK));
			//ImageList_AddIcon(m_himlSmall, hIcon);

			ListView_InsertItem(m_hwndList, &lvI);

			//m_pTaskBar->AddProgram(index, pShortcut->pszName);
//		}

		
		index++;
	}
}

CListView::~CListView()
{

}

void CListView::Move(int left, int top, int right, int bottom)
{
	MoveWindow( m_hwndList, left, top, right, bottom, TRUE);
}

void CListView::AddItem(LPSTR lpszText)
{
	//SendMessage (m_hwndList, LB_ADDSTRING,	0, (LPARAM) text) ;
	//SendMessage (hwndList, LB_SETITEMDATA,	i, (LPARAM) msgs[i].msg) ;
}
