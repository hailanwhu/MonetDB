/*
 * The contents of this file are subject to the MonetDB Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.monetdb.org/Legal/MonetDBLicense
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is the MonetDB Database System.
 *
 * The Initial Developer of the Original Code is CWI.
 * Portions created by CWI are Copyright (C) 1997-July 2008 CWI.
 * Copyright August 2008-2012 MonetDB B.V.
 * All Rights Reserved.
 */

/*
 * @f attach
 * @a Martin Kersten
 * @v 0.1
 * @+ Attach
 * The attach module provides a mechanism to easily share (readonly)
 * BATs between two mservers.
 *
 * The module is intended to support cheap attachment of BATs identified
 * using several protocols, but for the moment we assume a single Linux
 * cluster with NFS.
 * In this context the bind() creates a symbolic link.
 *
 * Beware that the argument to the patterns is the full path to
 * the base of the BAT. Depending on their type we need to
 * establish attachment to head,tail, hheap and theap.
 * The count argument is needed, because it can not be derived from the
 * file structure itself.
 *
 */
/*
 * @- Module initializaton
 */
#include "monetdb_config.h"
#include "attach.h"

/*
 * @- Operator implementation
 *
 */
str
ATTbind(Client cntxt, MalBlkPtr mb, MalStkPtr stk, InstrPtr pci)
{
#ifdef WIN32
	(void) cntxt;
	(void) mb;
	(void) stk;
	(void) pci;
	throw(MAL, "attach.bind", "Not available on Windows");
#else
	int *ret = (int*) getArgReference(stk, pci, 0);
	str fnme = *(str*) getArgReference(stk, pci, 1);
	int bid;

	(void) cntxt;
	(void) mb;
	bid = BBPimportEntry(fnme);
	if (bid ){
		BBPkeepref(*ret= bid);
		return MAL_SUCCEED;
	}
	throw(MAL, "attach.bind", INTERNAL_BAT_ACCESS);
#endif
}

str
ATTbindPartition(Client cntxt, MalBlkPtr mb, MalStkPtr stk, InstrPtr pci)
{
	int *ret,bid;
	BAT *b, *view;
	str msg= MAL_SUCCEED;
	BUN low,hgh;

	msg = ATTbind(cntxt,mb,stk,pci);
	if ( msg == MAL_SUCCEED){
		ret = getArgReference(stk,pci,0);
		bid = *(int*) getArgReference(stk,pci,0);
		low = (BUN) *(oid*) getArgReference(stk,pci,2);
		hgh = (BUN) *(oid*) getArgReference(stk,pci,3);

		if ((b = BATdescriptor(bid)) == NULL)
			throw(MAL, "attach.bind", INTERNAL_BAT_ACCESS);
		view = VIEWcreate_(b, b, TRUE);
		VIEWbounds(b, view, (BUN) low, hgh );
		BATseqbase(view, low);
		BBPkeepref(*ret = view->batCacheid);
		BBPunfix(b->batCacheid);
	}
	return msg;
}

str ATTlocation(str *fnme, int *bid)
{
	BAT *b = BBPquickdesc(*bid, FALSE);
	char path[BUFSIZ], *s;

	if ( b == NULL )
		throw(MAL,"attach.location",RUNTIME_OBJECT_UNDEFINED);

	snprintf(path,BUFSIZ,"%s%c%s%c",GDKgetenv("gdk_dbfarm"),DIR_SEP, GDKgetenv("gdk_dbname"), DIR_SEP);
	GDKfilepath(path+strlen(path), BATDIR, ( b->T->heap.filename ? b->T->heap.filename: b->H->heap.filename), 0);
	s= strrchr(path,'.');
	if( s) *s = 0;
	*fnme = GDKstrdup(path);
	return MAL_SUCCEED;
}

