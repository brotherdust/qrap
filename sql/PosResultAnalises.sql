create table antneuralnet (
id serial not null,
lastmodified timestamp without time zone,
machineid smallint references machine(id),
cellid integer references cell(id) on delete cascade,
filename text);

select * from antneuralnet;

drop table antneuralnet;

update cluttertype set height = 2 where landcoverid=38;

select * from cluttertype;

truncate table neuralnet cascade;
truncate table anninputlist cascade;
truncate table positionestimate cascade;


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


INSERT into PositionEstimate (id, tp, azimuth, distance, error) Values (52389,52389, 173.571006, 1817.37605, 0); 

select  delta, count(tp) from
(select servci, testpoint.id as tp, distance, floor(distance/553.5) as pta, ta, (ta - floor(distance/553.5)) as delta
from positionestimate 
cross join testpoint cross join testpointauxGSM
where positionestimate.tp=testpoint.id
and testpointauxGSM.tp=testpoint.id
and testpoint.positionsource=1) as temp
group by delta
order by delta;

delete from neuralnet where id<25;

delete from anninputlist where annid<25;
select * from neuralnet
order by siteid,id;

select delta, count(*) from
(select servci, testpoint.id as tp, distance, floor(distance/553.5) as pta, ta, (ta - floor(distance/553.5)) as delta
from positionestimate 
cross join testpoint cross join testpointauxGSM
where positionestimate.tp=testpoint.id
and testpointauxGSM.tp=testpoint.id
and testpoint.positionsource=1) as temp
group by delta
order by delta;

select count(*) from testpoint;

select * from positionestimate;

select * from testpointauxGSM;

select count(*) from test;

Create table TPtoDELETE
(tp bigint);

drop table TPtoDELETE;
 
insert into TPtoDELETE 
select TP1ID from
(select aux1.servci as sCI1, aux2.servci as sCI2, TP1.id as TP1ID, TP2.id as TP2ID, 
PE1.distance as Dist1, raw1.ta as TA1, PE2.distance as Dist2, raw2.ta as TA2,
PE1.azimuth as Azi1, PE2.azimuth as Azi2, 
abs(PE1.distance- PE2.distance) as delta, raw1.meastime, raw2.meastime, 
raw1.latitude, raw2.latitude, raw1.longitude, raw2.longitude,
raw1.bcch as BCCH1, raw1.bsic_num as BSIC1, raw1.cellid as CI1, 
raw1.neighborarfcn_1 as NB1BCCH1, raw1.neighbor_bsic_num_1 as NB1BSIC1, raw1.neighborcellid_1 as NB1CI1,
raw1.neighborarfcn_2 as NB2BCCH1 , raw1.neighborbsic_num_2 as NB2BSIC1, raw1.neighborcellid_2 as NB2CI1,
raw1.neighborarfcn_3 as NB3BCCH1, raw1.neighborbsic_num_3 as NB3BSIC1, raw1.neighborcellid_3 as NB3CI1,
raw2.bcch, raw2.bsic_num, raw2.cellid, 
raw2.neighborarfcn_1, raw2.neighbor_bsic_num_1, raw2.neighborcellid_1,
raw2.neighborarfcn_2, raw2.neighborbsic_num_2, raw2.neighborcellid_2,
raw2.neighborarfcn_3, raw2.neighborbsic_num_3, raw2.neighborcellid_3
 from seqTP as sTP1 
cross join seqTP as sTP2
cross join testpointauxGSM as aux1
cross join testpointauxGSM as aux2
cross join testpoint as TP1
cross join testpoint as TP2
cross join positionestimate as PE1
cross join positionestimate as PE2
cross join temprawgsmtems as raw1
cross join temprawgsmtems as raw2
where sTP1.seq=sTP2.seq+1
and sTP1.tp = TP1.id
and sTP2.tp = TP2.id
and TP1.id = PE1.tp
and TP2.id = PE2.tp
and sTP1.tp = aux1.tp
and sTP2.tp = aux2.tp
and raw1.id = TP1.id
and raw2.id = TP2.id
and raw1.neighbor_bsic_num_1 is null 
and raw1.neighborbsic_num_2 is null
and raw1.neighborbsic_num_3 is null
order by sTP1.seq
) as err
where dist1>=dist2;


select distinct servci from testpointauxGSM; 

delete from testpoint where id in
(select tp from TPtoDELETE);

select count(*) from positionestimate;
select * from testpointauxGSM;
select * from testpoint where originaltp is not null;

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

update results set aziErr= aziErr + 360
where aziErr < - 180;


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

select * from measurement;

drop table measedist;
create table measdist as
select meas, count(*) from
(select round(measvalue) as meas
from measurement) as temp
group by meas
order by meas;

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

delete from testpoint where id in
(select originaltp from
(select originaltp, averror from
(select originaltp, avg(error) as averror
from testpoint cross join positionestimate
where positionestimate.tp = testpoint.id
and positionsource=12
group by originaltp) as interim
where averror>1000) as buite)





select siteid, avg(error) 
from Results 
cross join cell cross join radioinstallation 
where positionsource=12
and servci=cell.id
and cell.risector= radioinstallation.id
group by siteid
order by siteid;


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


select positionsource, count(testpoint.id) from 
testpoint cross join positionestimate cross join 
(select originaltp, min(error) as minerror
from testpoint cross join positionestimate
where positionestimate.tp=testpoint.id
and positionsource<12 and positionsource>1
and originaltp is not null
group by originaltp) as minlist
where testpoint.originaltp = minlist.originaltp
and positionestimate.tp=testpoint.id
and error=minerror
and positionsource<12 and positionsource>1
group by positionsource
order by positionsource;

select * from positionestimate cross join testpoint where tp=testpoint.id
order by originaltp, positionsource;

insert into todeletetp ( 
select tp from
(select testpoint.id as tp, (ta*553.5+553.5/2-distance)/553.2 as delta
from positionestimate cross join testpoint
cross join testpointauxGSM
where testpointauxGSM.tp=testpoint.id
and positionestimate.tp=testpoint.id
order by delta desc) as binne
where delta>3); 

delete from testpoint where id in (select tp from todeletetp);

Select distinct id, landcoverid, height, width from 
(SELECT this.id as id, this.landcoverid as landcoverid, 
standard.height as height, standard.width as width 
FROM cluttertype as this CROSS JOIN cluttertype standard  
WHERE standard.id=this.standardtype  
AND this.classgroup=1 
UNION  
SELECT id, landcoverid, height, width  
FROM cluttertype WHERE standardtype is NULL  AND classgroup=1 ) 
as alles order by landcoverid;