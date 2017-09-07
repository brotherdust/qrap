select count(*) from test;

delete from testpoint where positionsource>1;

select * from anninputlist;

truncate table neuralnet cascade;
truncate table anninputlist cascade;
truncate table positionestimate cascade;

create table Results as
select servci, tp1.id as tp, tp2.id as origtp, tp1.positionsource, 
(pe1.azimuth-pe2.azimuth) as aziErr, 
abs(pe1.Distance-pe2.Distance) as distErr, pe1.error as Error
from testpoint as tp1 cross join positionestimate as pe1
cross join testpoint as tp2 cross join positionestimate as pe2
cross join testpointauxGSM
where tp1.positionsource>1
and tp1.originaltp = tp2.id
and tp2.id = testpointauxGSM.tp
and pe1.tp = tp1.id
and pe2.tp = tp2.id;

drop table Results;

select * from Results;

drop table AziErrorDistribution;

update results set aziErr= aziErr - 360
where aziErr > 180;
update results set aziErr= aziErr + 360
where aziErr <- 180;

update positionestimate set Distance = abs(Distance);

create table AziErrorDistribution as
select positionsource, aziError, count(*) from 
(select positionsource, tp, round(azierr) as aziError
from Results) as temp
group by positionsource, aziError
order by positionsource, aziError;

drop table AziOnsSidedErrorDistribution;

create table AziOnsSidedErrorDistribution as
select positionsource, aziError, count(*) from 
(select positionsource, tp, 2*round(abs(azierr)/2) as aziError
from Results) as temp
group by positionsource, aziError
order by positionsource, aziError;

drop table DistErrorDistribution;

create table DistErrorDistribution as
select positionsource, distError, count(*) from 
(select positionsource, tp, 5*round(disterr/5) as distError
from Results) as temp
group by positionsource, distError
order by positionsource, distError;

drop table errordistribution;

create table ErrorDistribution as
select positionsource, Error, count(*) from 
(select positionsource, tp, 5*round(error/5) as Error
from Results) as temp
group by positionsource, Error
order by positionsource, Error;

create table ANNerrorDistribution as
select siteid, Error, count(*) from 
(select siteid, tp, 10*round(error/10) as Error
from Results 
cross join cell cross join radioinstallation 
where positionsource=12
and servci=cell.id
and cell.risector= radioinstallation.id) as temp
group by siteid, Error
order by siteid, Error;


SELECT distinct servrad.siteid as ssite, tp2.servci as servci, measurement.tp as tp,  
measurement.id as mid, ST_AsText(testpoint.location) as origLocation, rad.siteid as nsite, 
ST_AsText(site.location) as nsiteLocation, rad.id as InstKeyFixed, rad.txantennaheight as antheight, 
rad.EIRP as EIRP, rad.txbearing as bearing, rad.txmechtilt as tilt, rad.txantpatternkey as antkey, 
azibeamwidth, ci, measurement.frequency as frequency, max(measvalue) as measvalue, 
min(pathloss) as pathloss, tp1.TA as TA, technology.DistRes as DistRes 
from measurement left outer join test as tp1 
on (measurement.tp =tp1.tp and measurement.ci = tp1.servci) cross join testpoint 
cross join test as tp2 cross join cell as servcell cross join radioinstallation as servrad 
cross join site as servsite cross join cell cross join radioinstallation as rad 
cross join antennapattern cross join site LEFT OUTER JOIN technology ON (rad.techkey=technology.id) 
where measurement.tp=testpoint.id and measurement.ci=cell.id and rad.siteid=site.id 
and measurement.tp = testpoint.id and testpoint.id=tp2.tp and tp2.servci=servcell.id 
and servcell.risector=servrad.id and servrad.siteid=servsite.id and cell.risector=rad.id 
and rad.txantpatternkey = antennapattern.id and testpoint.positionsource<2 
and measurement.tp in (select tp from test) 
and servsite.Location @ ST_GeomFromText('POLYGON((28.21 -25.965,28.245 -25.97,28.255 -26.015,28.175 -26.062,28.15 -25.99,28.18 -25.965,28.21 -25.965))',4326)  
and meastype=2 and measdatasource=6 and positionsource=1 and Technology.id=1 
and measurement.tp=90005
group by ssite, tp2.servci, nsite, mid, InstKeyFixed, azibeamwidth, origLocation,  
measurement.frequency, measurement.tp, tp1.TA, ci, nsiteLocation,  DistRes  
order by measurement.tp, TA, measvalue desc;

SELECT distinct measurement.tp as tp, ST_AsText(testpoint.location) as origLocation, siteid as nsite, 
ST_AsText(site.location) as nsiteLocation, radioinstallation.id as InstKeyFixed, 
txantennaheight as antheight, EIRP, txbearing as bearing, txmechtilt as tilt, 
txantpatternkey as antkey, azibeamwidth, ci, ST_AsText(centriod) as centriod, 
measurement.frequency as frequency, max(measvalue) as measvalue, min(pathloss) as pathloss, 
tp1.TA as TA, technology.DistRes as DistRes 
from measurement left outer join test as tp1 on ( measurement.tp =tp1.tp and measurement.ci = tp1.servci) 
cross join testpoint cross join cell cross join radioinstallation  
cross join antennapattern cross join site 
LEFT OUTER JOIN technology ON (radioinstallation.techkey=technology.id) 
where measurement.tp=testpoint.id and measurement.ci=cell.id and siteid=site.id 
and cell.risector=radioinstallation.id and radioinstallation.txantpatternkey = antennapattern.id 
and testpoint.positionsource<2 and measurement.tp in (select tp from test) 
and site.Location @ ST_GeomFromText('POLYGON((28.21 -25.965,28.245 -25.97,28.255 -26.015,28.175 -26.062,28.15 -25.99,28.18 -25.965,28.21 -25.965))',4326)  
and meastype=2 and measdatasource=6 and positionsource=1 and Technology.id=1 
and measurement.tp=90005
group by siteid, InstKeyFixed, azibeamwidth, origLocation,  
measurement.frequency, measurement.tp, tp1.TA, ci, nsiteLocation, centriod, DistRes  
order by measurement.tp, TA, measvalue desc;

