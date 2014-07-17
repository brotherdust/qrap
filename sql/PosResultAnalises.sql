select servci, tp from
(select servci, testpoint.id as tp, distance, floor(distance/553.5) as pta, ta, (ta - floor(distance/553.5)) as delta
from positionestimate 
cross join testpoint cross join testpointauxGSM
where positionestimate.tp=testpoint.id
and testpointauxGSM.tp=testpoint.id
and testpoint.positionsource=1) as temp
where delta <0;


select delta, count(*) from
(select servci, testpoint.id as tp, distance, floor(distance/553.5) as pta, ta, (ta - floor(distance/553.5)) as delta
from positionestimate 
cross join testpoint cross join testpointauxGSM
where positionestimate.tp=testpoint.id
and testpointauxGSM.tp=testpoint.id
and testpoint.positionsource=1) as temp
group by delta
order by delta;


select * from positionestimate;
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

select * from Results;

create table AziErrorDistribution as
select positionsource, aziError, count(*) from 
(select positionsource, tp, 2*round(azierr/2) as aziError
from Results) as temp
group by positionsource, aziError
order by positionsource, aziError;

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
where positionsource =6) as tplist
where origtp=otp;

select positionsource, count(*) as num from
(select cosruleResults.origtp, positionsource, minerror
from cosruleResults cross join
(select origtp, min(abs(error)) as minerror
from cosruleResults 
group by origtp) as minerr
where abs(error)<=minerror 
and minerr.origtp = cosruleresults.origtp
and positionsource<>12) as opsom
group by positionsource
order by num desc;

select * from results;
