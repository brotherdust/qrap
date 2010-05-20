
drop table links;

create table links 
(id	serial primary key unique not null,
machineid 	smallint,
linkname	varchar(150),
radioinstallation1 integer references radioinstallation(id) on delete cascade,
radioinstallation2 integer references radioinstallation(id) on delete cascade,
minclearance real,
frequency real,
rxlev real,
kfactor real);
SELECT AddGeometryColumn('public', 'links', 'line', 4326, 'LINESTRING', 2);

insert into site (groundheight,lastmodified,location,machineid,sitename,status) values (0,now(),GeomFromText('POINT(26.000000 -26.000000)', 4326),1,'30GHz','Experimental');
