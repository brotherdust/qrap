select count(*), t1.positionsource, avg( ST_distance(t2.location, t1.location)) as error
from testpoint as t1 cross join testpoint as t2 
where t1.originaltp = t2.id
and t2.measdatasource=5
group by t1.positionsource
order by t1.positionsource;

select positionsource, count(*) as num from
positionestimate cross join testpoint cross join
(select testpoint.originaltp, min(error) as minE from
positionestimate cross join testpoint
where positionestimate.tp=testpoint.id
and testpoint.originaltp in (select originaltp from testpoint where positionsource=10)
and positionsource>1
group by testpoint.originaltp) as minError
where positionestimate.tp=testpoint.id
and error=minE
and testpoint.originaltp= minError.originaltp
group by positionsource
order by num desc;

select positionsource, avg(abs(AO1.azimuth-AOA.azimuth)) as AziError,
avg(abs(AO1.distance-AOA.distance)) as DistError, avg(AOA.error) as Error
from
(select testpoint.id as id, azimuth, distance, error
from positionestimate cross join testpoint
where tp=testpoint.id
and positionsource=1) as AO1
cross join
(select positionsource, originaltp, azimuth, distance, error
from positionestimate cross join testpoint
where tp=testpoint.id
and positionsource>1) as AOA
where AOA.originaltp=AO1.id
group by positionsource
order by DistError;


select * from positionsource;

delete from testpoint where id in
(select t1.id 
from testpoint as t1 cross join testpoint as t2 
where t1.originaltp = t2.id
and t2.measdatasource=5
and  ST_distance(t2.location, t1.location)>0.11);
and t1.positionsource>1);

delete from testpoint where id not in
(select id from testpoint where id in
(select tp from testpointauxGSM)
or measdatasource<>5);


truncate table PositionEstimate;

delete from testpoint where positionsource>1;

INSERT into PositionEstimate (id, tp, azimuth, distance, error) Values (618656,618656, -62.5, 0, 0);