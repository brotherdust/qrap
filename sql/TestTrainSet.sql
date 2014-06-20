create view tplist as
(select tp, servci, RxTxDiff as ta
from testpointauxUTRAN
where RxTxDiff is not null
union
select tp, servci, RxTxDiff2 as ta
from testpointauxUTRAN
where RxTxDiff2 is not null
union
select tp, servci, ta
from testpointauxGSM);


select distinct siteid from
(
select distinct siteid, min(txbearing) as antbearing, ci, testpoint.id as tp, 
ST_AsText(testpoint.location) as origLocation, measurement.id as mid, measvalue, 
frequency, EIRP, tp1.TA as TA, technology.DistRes as DistRes 
from measurement left outer join tplist as tp1 
on ( measurement.tp =tp1.tp and measurement.ci = tp1.servci) 
cross join testpoint cross join tplist as tp2 cross join cell 
cross join radioinstallation cross join technology 
where measurement.tp=testpoint.id and tp2.servci = cell.id 
and testpoint.id = tp2.tp and cell.risector = radioinstallation.id 
and radioinstallation.techkey = technology.id and testpoint.positionsource<2 
and testpoint.location @ ST_GeomFromText('POLYGON((28.26 -26.06,28.26 -25.94,28.113 -25.94,28.113 -26.06,28.26 -26.06))',4326)  
and meastype=2 and measdatasource=6 and positionsource=1 and Technology.id=1 
group by siteid, testpoint.id, tp1.TA, mid, ci, measvalue, frequency, EIRP, DistRes 
order by siteid, tp, TA, measvalue desc) as alltp;



drop table sitelist;

create table sitelist as
select siteid from
(select siteid, sitelocation, numPoints from
(select siteid, ST_AsText(site.location) as siteLocation, count(tp) as numPoints
from testpointauxGSM cross join cell
cross join radioinstallation
cross join site
where servci=cell.id
and cell.risector = radioinstallation.id
and radioinstallation.siteid = site.id
group by siteid, site.location
order by numPoints desc) as binne
where numPoints>2000) as buite;

select measvalue, count(*) from measurement
group by measvalue
order by measvalue;


create view sitesFound as
select siteid, sitelocation, numPoints from
(select siteid, site.location as siteLocation, count(tp) as numPoints
from testpointauxGSM cross join cell
cross join radioinstallation
cross join site
where servci=cell.id
and cell.risector = radioinstallation.id
and radioinstallation.siteid = site.id
group by siteid, site.location
order by numPoints desc) as binne
where numPoints>2000;

drop view sitesFound;
 

SELECT testpoint.id as tp, 
siteid, servci,
from testpoint cross join testpointauxGSM 
cross join cell cross join radioinstallation 
where testpointauxGSM.servci=cell.id 
and cell.risector=radioinstallation.id 
and testpoint.id=testpointauxGSM.tp
and testpoint.positionsource<2
order by siteid, servci, tp;


select distinct siteid, ST_AsText(site.location) as siteLocation,
min(tGSM.servci) GSMservci, min(tUMTS.servci) as UMTSservci,  
min(txbearing) as antbearing, ci, testpoint.id as tp, 
ST_AsText(testpoint.location) as origLocation,
measurement.id as mid, measvalue, frequency, EIRP,
min(tUMTS.RxTxDiff) as timeDiff1, min(tUMTS.RxTxDiff2) as timeDiff2,
min(tGSM.TA) as TA, technology.DistRes as DistRes
from measurement left outer join testpointauxGSM as tGSM
on ( measurement.tp =tGSM.tp and measurement.ci = tGSM.servci)
left outer join testpointauxUTRAN as tUMTS
on ( measurement.tp = tUMTS.tp and measurement.ci = tUMTS.servci)
cross join testpoint cross join cell cross join radioinstallation 
cross join technology cross join site
where measurement.tp=testpoint.id and measurement.ci = cell.id
and cell.risector = radioinstallation.id 
and radioinstallation.techkey = technology.id and siteid=site.id
group by siteid, site.location,testpoint.id, mid, ci, measvalue, frequency, EIRP, DistRes
order by siteid, GSMservci, UMTSservci, tp, TA, timeDiff1, timeDiff2, measvalue;


select siteid, sitelocation, ci from
(select siteid, ST_AsText(site.location) as siteLocation,
ci, count(measurement.tp) as numPoints
from measurement cross join testpointauxGSM cross join cell
cross join radioinstallation cross join site
where servci=cell.id
and cell.risector = radioinstallation.id
and measurement.tp=testpointauxGSM.tp
and siteid =site.id
group by siteid, ci, site.location
order by siteid, numPoints desc) as lys
where numPoints>190
order by siteid, numPoints desc;



select siteid, count(ci) as num from
(select distinct siteid, ci
from measurement cross join testpointauxGSM cross join cell
cross join radioinstallation
where servci=cell.id
and cell.risector = radioinstallation.id
and measurement.tp=testpointauxGSM.tp) as found
group by siteid
order by num desc;

