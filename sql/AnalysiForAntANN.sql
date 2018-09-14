create table AnalysisForAntANN as
select measurement.*, (measvalue - predictvalue) as delta, (32.45 +20*log(frequency) + 20*log(distance)) as freespace,
(pathloss - 32.45 -20*log(frequency) - 20*log(distance)) as obstructionloss 
from measurement;

drop table AnalysisForAntANN


select count (*) from measurement where distance <1 ;

delete from measurement where distance = 0;

select avg(delta) as mean, stddev(delta) as sigma, count(*) as num
from AnalysisForAntANN where CI=233
and obstructionloss >=-30;