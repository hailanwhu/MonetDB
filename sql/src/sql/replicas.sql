-- The contents of this file are subject to the MonetDB Public License
-- Version 1.1 (the "License"); you may not use this file except in
-- compliance with the License. You may obtain a copy of the License at
-- http://monetdb.cwi.nl/Legal/MonetDBLicense-1.1.html
--
-- Software distributed under the License is distributed on an "AS IS"
-- basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
-- License for the specific language governing rights and limitations
-- under the License.
--
-- The Original Code is the MonetDB Database System.
--
-- The Initial Developer of the Original Code is CWI.
-- Copyright August 2008-2009 MonetDB B.V.
-- All Rights Reserved.


-- The replicator information is stored in the table 'replicas'

create table sys.replicas(id int, stamp timestamp);

CREATE TABLE sys.slaves (
	uri              varchar(100)   NOT NULL,
	last_connect     timestamp,
	last_disconnect  timestamp,
	logfile          varchar(100),
	tag_delay        bigint,
	time_delay       timestamp
);
