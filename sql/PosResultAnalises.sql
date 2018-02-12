delete from measurement where frequency < 500;

delete from testpoint where id not in (select tp from measurement);

delete from testpointauxlte where tp not in 
(select id from testpoint where
location @ ST_GeomFromText('POLYGON((27.9750 -26.0350,
28.0750 -26.0350,
28.0750 -26.109,
27.9750 -26.109,
27.9750 -26.0350))',4326));

delete from testpointauxLTE where servci not in 
(select cell.id 
from cell cross join radioinstallation cross join site
where risector=radioinstallation.id
and siteid= site.id and
location @ ST_GeomFromText('POLYGON((28.0218767 -26.04847747,
28.00377964 -26.06021314,
28.01408948 -26.08763293,
28.05653532 -26.08927812,
28.05181911 -26.06898747,
28.03898665 -26.05790988,
28.0218767 -26.04847747))',4326));

delete from testpoint where id not in (select tp from testpointauxLTE);




delete from testpoint where positionsource>1;

select * from anninputlist;

truncate table neuralnet cascade;
truncate table anninputlist cascade;
truncate table positionestimate cascade;

drop table Results;

create table Results as
select servci, tp1.id as tp, tp2.id as origtp, tp1.positionsource, 
(pe1.azimuth-pe2.azimuth) as aziErr, 
abs(pe1.Distance-pe2.Distance) as distErr, pe1.error as Error
from testpoint as tp1 cross join positionestimate as pe1
cross join testpoint as tp2 cross join positionestimate as pe2
cross join testpointauxLTE
where tp1.positionsource>1
and tp1.originaltp = tp2.id
and tp2.id = testpointauxLTE.tp
and pe1.tp = tp1.id
and pe2.tp = tp2.id;

update results set aziErr= aziErr - 360
where aziErr > 180;
update results set aziErr= aziErr + 360
where aziErr <- 180;

update positionestimate set Distance = abs(Distance);

drop table AziErrorDistribution;

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
(select positionsource, tp, 2*round(disterr/2) as distError
from Results) as temp
group by positionsource, distError
order by positionsource, distError;

drop table errordistribution;

create table ErrorDistribution as
select positionsource, Error, count(*) from 
(select positionsource, tp, 2*round(error/2) as Error
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

select num, count(*) from
(select tp, count(*) as num 
from measurement
group by tp) as binne
group by num
order by num;

create table LTEnumPerTP as
select measurement.tp as tp, count(*) as num 
from measurement cross join tpused
where measurement.tp=tpused.tp
group by measurement.tp;

drop table GSMmeasCountDist;
create table LTEmeasCountDist as
select num, count(*) as aantal
from LTEnumPerTP
group by num;

select * from LTEmeasCountDist
order by num;
