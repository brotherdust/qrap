delete from site where status is null;

alter table all_kgalagadi_schools_v3a rename column "INSTITUTIO" to schoolname;

insert into site (sitename, location) select schoolname, SetSRID(the_geom,4326) from all_kgalagadi_schools_v3a;

select * from site; 

select * from all_kgalagadi_schools_v3a; 

truncate table links;

delete from radioinstallation where id > 4;


update radioinstallation set 
techkey=2, 
eirp=27, 
txpower=27, 
txantennaheight=3,
txantpatternkey=11,
rxsensitivity=-69,
rxantennaheight=3,
rxantpatternkey=11
where project is null
and siteid not in 
(select siteid from radioinstallation where project in (0,1)); 

delete from radioinstallation where techkey is null;
commit;



