SELECT distinct measurement.tp as tp, ST_AsText(testpoint.location) as origLocation,siteid, 
ST_AsText(site.location) as siteLocation, radioinstallation.id as InstKeyFixed, txantennaheight, 
EIRP, txbearing, txmechtilt, txantpatternkey, azibeamwidth, ci, ST_AsText(centriod) as centriod, 
measurement.frequency as frequency, max(measvalue) as measvalue, min(pathloss) as pathloss, tp1.TA as TA, 
technology.DistRes as DistRes 
from measurement left outer join tplist as tp1 on ( measurement.tp =tp1.tp and measurement.ci = tp1.servci) 
cross join testpoint cross join cell cross join radioinstallation  cross join antennapattern 
cross join site LEFT OUTER JOIN technology ON (radioinstallation.techkey=technology.id) 
where measurement.tp=testpoint.id and measurement.ci=cell.id and siteid=site.id 
and cell.risector=radioinstallation.id and radioinstallation.txantpatternkey = antennapattern.id 
and testpoint.positionsource<2 
and site.Location @ ST_GeomFromText('POLYGON((28.26 -26.06,28.26 -25.94,28.113 -25.94,28.113 -26.06,28.26 -26.06))',4326)  
and meastype=2 and measdatasource=6 and positionsource=1 and Technology.id=1 
group by siteid, InstKeyFixed, azibeamwidth, origLocation,  measurement.frequency, measurement.tp, tp1.TA, ci, siteLocation, centriod, DistRes  
order by measurement.tp, TA, measvalue desc;

update antennapattern set azibeamwidth=360 where id>315 and id <344;

INSERT into PositionEstimate (id, tp, azimuth, distance, error) Values (52389,52389, 173.571006, 1817.37605, 0); 



delete from testpoint where id in 
(select  tp from
(select servci, testpoint.id as tp, distance, floor(distance/553.5) as pta, ta, (ta - floor(distance/553.5)) as delta
from positionestimate 
cross join testpoint cross join testpointauxGSM
where positionestimate.tp=testpoint.id
and testpointauxGSM.tp=testpoint.id
and testpoint.positionsource=1) as temp
where delta <0);


select delta, count(*) from
(select servci, testpoint.id as tp, distance, floor(distance/553.5) as pta, ta, (ta - floor(distance/553.5)) as delta
from positionestimate 
cross join testpoint cross join testpointauxGSM
where positionestimate.tp=testpoint.id
and testpointauxGSM.tp=testpoint.id
and testpoint.positionsource=1) as temp
group by delta
order by delta;

update testpointauxGSM set ta = floor(distance/553.5) from
positionestimate where testpointauxGSM.tp =positionestimate.tp;

select * from technology;

update technology set distres=553.5 where distres>=554;


select count(*) from positionestimate;
select * from testpointauxGSM;
select * from testpoint where originaltp is not null;

create table Results as
select servci, tp1.id as tp, tp2.id as origtp, tp1.positionsource, 
(pe1.azimuth-pe2.azimuth) as aziErr, 
(pe1.Distance-pe2.Distance) as distErr, pe1.error as Error
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

create table AziErrorDistribution as
select positionsource, aziError, count(*) from 
(select positionsource, tp, 2*round(azierr/2) as aziError
from Results) as temp
group by positionsource, aziError
order by positionsource, aziError;

create table DistErrorDistribution as
select positionsource, distError, count(*) from 
(select positionsource, tp, 20*round(disterr/20) as distError
from Results) as temp
group by positionsource, distError
order by positionsource, distError;

create table CosRuleErrorDistribution as
select positionsource, Error, count(*) from 
(select positionsource, tp, 10*round(error/10) as Error
from CosruleResults) as temp
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


select positionsource, count(*) as num from
(select results.origtp, positionsource, minerror
from results cross join
(select origtp, min(error) as minerror
from results where  positionsource<>12
group by origtp) as minerr
where error=minerror and minerr.origtp = results.origtp
and  positionsource<>12) as opsom
group by positionsource
order by num desc;

drop table cosruleResults;

create table cosruleResults as
select * from results cross join
(select origtp as otp from results 
where positionsource = 5) as tplist
where origtp=otp;

select positionsource, count(*) as num from
(select cosruleResults.origtp, positionsource, minerror
from cosruleResults cross join
(select origtp, min(abs(error)) as minerror
from cosruleResults 
group by origtp) as minerr
where abs(error)<=minerror 
and minerr.origtp = cosruleresults.origtp) as opsom
group by positionsource
order by num desc;

select * from neuralnet
order by siteid;

delete from anninputlist where annid in (26,27,28,29);
