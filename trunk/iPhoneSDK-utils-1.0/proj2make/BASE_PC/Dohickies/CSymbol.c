/*************************************************************************																		**		Symbol Table Manager - Jim Schimpf Pandora Products				*			*  																		**		MAC FILE LEXICAL ANALYZER FOR Mac Project Builder Files			**																		**		Module:CSymbol.c												**																		**	Version		Date	Person  	Description							**-----------------------------------------------------------------------**	0.1		28-Jul-2002	J.Schimpf	Initial Version						**	0.2		30-Jul-2002 J.Schimpf	Fix add function to store input vals**																		**	DESCRIPTION:													    **		This class creates and manages a symbol hash table.  It allows	**	insertion and find of text symbols.  Each symbol has a void ptr		**	attached which contains arbitrary data.								**																		**																		*************************************************************************/#include "CSymbol.h"/***********************************************************************						*** INITIALIZATION **************************************************************************//*************************************************************************  CSymbol(void)	- Class constructor**	INPUT:	NONE**	OUTPUT:	NONE*			Set to default size and start table************************************************************************/CSymbol::CSymbol(void){	size = DEF_HTBL_SIZE;	this->init();}/*************************************************************************  CSymbol(int hash_size)	- Class constructor**	INPUT:	hash_size	- User defined size**	OUTPUT:	NONE*			Set to default size and start table************************************************************************/CSymbol::CSymbol(int hash_size){	size = hash_size;	this->init();}/*************************************************************************  ~CSymbol(void)	- Class destructor**	INPUT:	NONE**	OUTPUT:	NONE*			Delete all internal structures************************************************************************/CSymbol::~CSymbol(void){	int i;	HASH_EL *el,*next;		// Loop through and delete the attached hash elements		for( i=0; i<size; i++ )	{		if( list[i] != NULL )		{			el = list[i];			while( el != NULL )			{				next = el->next;		// Save the next ptr								// Delete the parts				free(el->name);				free( el);								el = next;			}		}				// Now delete the list				delete list;	}}/***********************************************************************						*** OPERATION **************************************************************************//*************************************************************************  bool add( char *name,void *ptr)	- Add element to symbol table**	INPUT:	name	- Symbol text name*			ptr		- Symbol value**	OUTPUT:	TRUE if added, FALSE if not************************************************************************/bool CSymbol::add(char *name,void *ptr){	HASH_EL *el;		// Two cases:	//	a) We already have the symbol, in that case just add the ptr	//	b) We do not so create element and add it on		el = this->find_internal( name );		if( el != NULL )	{		// First case, we have the element so just change the pointer				el->value = ptr;		return( true );	}		// Second case, not in table so create a new element		el = (HASH_EL *)calloc( 1,sizeof( HASH_EL ) );	if( el == NULL )		return( false );			el->name = (char *)calloc(1,strlen(name)+1);	if( el->name == NULL )	{		free( el );		return( false );	}		// Put the data into the hash element		strcpy(el->name,name);		// Put the name into the hash element (forgot...)	el->value = ptr;		// Add it in		this->add_internal( el );	return( true );}/*************************************************************************  void *find( char *name)	- Find the value for the symbol**	INPUT:	name	- Symbol for search**	OUTPUT:	Value ptr if found, NULL if not************************************************************************/void *CSymbol::find(char *name){	HASH_EL *el;		// (1) Find the hash element, if none then return NULL		el = this->find_internal( name );	if( el == NULL )		return( NULL );			// (2) We have a value, so return the value		return( el->value );}	/***********************************************************************						*** PRIVATE CODE **************************************************************************//*************************************************************************  init(void)	- Create symbol table hash list**	INPUT:	NONE**	OUTPUT:	NONE*			Hash list created************************************************************************/void CSymbol::init(void){	// Allocate the hash list as a list of 	// pointers to hasn elements	// The class variable size is the size of this array		list = (HASH_EL **) calloc( sizeof( HASH_EL *) , size );}/************************************************************************* HASH_EL *find_internal( char *name)	- Find the hash element for the symbol**	INPUT:	name	- Symbol for search**	OUTPUT:	HASH_EL for this symbol if found, NULL if none************************************************************************/HASH_EL *CSymbol::find_internal(char *name){	HASH_EL *el;	int hash;		// (1) Computer the hash value for this symbol and get the head	// of the matching list of hash elements		hash = this->hash_value( name );	el = list[hash];		// (2) Now search this list for a match, if NONE then	// return NULL		while( el != NULL )	{		if( strcmp(el->name,name) == 0 )			break;					el = el->next;	}		return( el );}/************************************************************************* void add_internal( HASH_EL *el)	- Add in a hash element**	INPUT:	eladd	- Hash element to be added (not in table now)**	OUTPUT:	NONE*			element added in to correct psn************************************************************************/void CSymbol::add_internal(HASH_EL *eladd){	int hash;		// (1) Computer the hash value for this symbol		hash = this->hash_value( eladd->name );		// (2) Now put new element into this slot		eladd->next = list[hash];		// Get rest of list	list[hash] = eladd;			// Make new head	}/*****************************************************************************  int hash_value(char *name)	- Generate HASH code from name**	INPUT:	name	- Symbol name**	OUTPUT:	Hash code from this name, 0-255****************************************************************************/int CSymbol::hash_value(char *name){	int i;	int hash;		/*  (1) Generate value from list by sum of letters in the		name and shifts	*/		for( i=0,hash=0; name[i] != '\0'; i++ )	{		hash = hash + (int)name[i];		hash = hash<<1;	}		/*  (2) Trim to size */		hash %= size;		return( hash );}