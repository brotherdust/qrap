create table distribution as 
select tp, areaname, timeofmeas, ci, measvalue
from measurement cross join testpoint cross join customareafilter
where tp=testpoint.id
and location@the_geom
and areaname<>'GautengClutterOutline'
order by areaname, ci, measvalue;

drop table distribution;

select areaname, count(*) as num from 
(select distinct areaname, tp 
from distribution) as binne
group by areaname
order by num desc;

select areaname, ci, min(measvalue), max(measvalue), 
avg(measvalue), stddev(measvalue), count(measvalue) as num 
from distribution 
group by areaname, ci
order by areaname, num desc;