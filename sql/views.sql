create view links_view as select links.id as id, linkname, 
			site1.sitename||rad1.sector as txsite, txinst, rad1.txantennaheight as txheight, rad1.txbearing as txbearing,
			site2.sitename||rad2.sector as rxsite, rxinst, rad2.rxantennaheight as rxheight, rad2.rxbearing as rxbearing,
			minclearance as clear,
			frequency, 
			ST_Distance( site1.location::geography, site2.location::geography)/1000 as distance, 
			pathloss, kfactor, line
			from links cross join radioinstallation as rad1 cross join site_view_only as site1
	 		cross join radioinstallation as rad2 cross join site_view_only as site2
	 		where txinst=rad1.id
			and rxinst=rad2.id
			and rad1.siteid=site1.id
			and rad2.siteid=site2.id;





create view site_view as select * from site;
create view site_view_only as select * from site;
create view site_view_list as select * from site;

create view equipmenttype_view as select * from equipment;
		
create view antennapattern_view as select * from antennapattern;

drop view radioinstallation_view cascade;			
create view radioinstallation_view as 
	select sitename, sitename||sector as sectorname, 
	radioinstallation.* from radioinstallation cross join site 
	where siteid=site.id;
							
							
create view antenna_view 
as select	antenna.id as id,
		site.sitename as sitename,
		antenna.serialnumber as serialnumber,
		antenna.rikey as rikey,
		antenna.antdevicekey as antdevicekey,
		antenna.installationdate as installationdate
		from antenna, radioinstallation, site 
		where (antenna.rikey=radioinstallation.id 
		and radioinstallation.siteid=site.id )
				
		union
		
		select	antenna.id as id,
		null as sitename,
		antenna.serialnumber as serialnumber,
		antenna.rikey as rikey,
		antenna.antdevicekey as antdevicekey,
		antenna.installationdate as installationdate
		from antenna where antenna.rikey is null;
		
		
create view equipment_view 
as select	equipment.id as id,
		site.sitename as sitename,
		equipment.serialnumber as serialnumber,
		equipment.rikey as rikey,
		equipment.equipkey as equipkey,
		equipment.installationdate as installationdate
		from equipment, radioinstallation,site 
		where (equipment.rikey=radioinstallation.id 
		and radioinstallation.siteid=site.id )
				
		union
				
		select	equipment.id as id,
		null as sitename,
		equipment.serialnumber as serialnumber,
		equipment.rikey as rikey,
		equipment.equipkey as equipkey,
		equipment.installationdate as installationdate
		from equipment where equipment.rikey is null;
		
		
create view cell_view as select 
	cell.id as id,
	site.sitename as sitename,
	radioinstallation.sector as sector,
	sitename||sector as sectorname,
	cell.risector as risector,
	cell.mastercell as mastercell,
	cell.permanent as permanent,
	cell.cstraffic as cstraffic,
	cell.pstraffic as pstraffic,
	cell.txpower as txpower,
	cell.beacon as beacon,
	cell.layerthreshold as layerthreshold
	from site, radioinstallation, cell
	where (cell.risector = radioinstallation.id and radioinstallation.siteid = site.id)
	union
	select	cell.id as id,
	null as sitename,
	null as sector,
	null as sectorname,
	cell.risector as risector,
	cell.mastercell as mastercell,
	cell.permanent as permanent,
	cell.cstraffic as cstraffic,
	cell.pstraffic as pstraffic,
	cell.txpower as txpower,
	cell.beacon as beacon,
	cell.layerthreshold as layerthreshold
	from cell where cell.risector is null;
				
				
 create view frequencyallocationlist_view as select 
	frequencyallocationlist.id as id,
	site.sitename as sitename,
	radioinstallation.sector as sector,
	frequencyallocationlist.ci as ci,
	frequencyallocationlist.carrier as carrier,
	frequencyallocationlist.channel as channel 
	from frequencyallocationlist, cell, radioinstallation,site 
	where (frequencyallocationlist.ci=cell.id 
	and cell.risector=radioinstallation.id 
	and radioinstallation.siteid=site.id )
		
	union
			
	select	frequencyallocationlist.id as id,
	null as sitename,
	null as sector,
	frequencyallocationlist.ci as ci,
	frequencyallocationlist.carrier as carrier,
	frequencyallocationlist.channel as channel
	from frequencyallocationlist where frequencyallocationlist.ci is null;

drop view links_view;
	
create view links_view as select links.id as id, linkname, 
			site1.sitename||rad1.sector as txsite, txinst, rad1.txantennaheight as txheight, rad1.txbearing as txbearing,
			site2.sitename||rad2.sector as rxsite, rxinst, rad2.rxantennaheight as rxheight, rad2.rxbearing as rxbearing,
			minclearance as clear,
			frequency, 
			ST_Distance( site1.location::geography, site2.location::geography)/1000 as distance, 
			pathloss, kfactor, line
			from links cross join radioinstallation as rad1 cross join site_view_only as site1
	 		cross join radioinstallation as rad2 cross join site_view_only as site2
	 		where txinst=rad1.id
			and rxinst=rad2.id
			and rad1.siteid=site1.id
			and rad2.siteid=site2.id;
			

	
create view radioinstallation_view as select 
site_view.sitename as sitename,  sitename||sector as sectorname, 
radioinstallation.* from radioinstallation cross join site_view  
cross join projects  where siteid=site_view_list.id  
and ((project=projects.id and projectname='North West')  or (project is NULL))	
	
create view site_view_only as 
select  from site_view_list  cross join radioinstallation  cross join projects  
where siteid=site_view_list.id  and ((project=projects.id and projectname='North West')  or (project is NULL)); 
union select * from site_view_list  where site_view_list.id not in (select distinct siteid from radioinstallation);

 create view site_view as select  site_view_list.id, site_view_list.lastmodified,  site_view_list.machineid, site_view_list.sitename, site_view_list.status, site_view_list.groundheight, site_view_list.location  from site_view_list cross join radioinstallation  cross join projects  where siteid=site_view_list.id  and ((project=projects.id and projectname='North West')  or (project is NULL)) union select * from site_view  where site_view_list.id not in (select distinct siteid from radioinstallation);