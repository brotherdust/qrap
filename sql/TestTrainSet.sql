select distinct site.id as siteid, ST_AsText(location) as siteLocation, type, maxdist, 
NumInputs, NumOutputs, filename, index, cellid from site cross join NeuralNet cross join anninputlist where NeuralNet.siteid=site.id and anninputlist.annid= NeuralNet.id and anninputlist.siteid=site.id and site.location  @ ST_GeomFromText('POLYGON((28.26 -26.06,28.26 -25.94,28.113 -25.94,28.113 -26.06,28.26 -26.06))',4326)  order by siteid, type, index; 

INSERT into testpoint (id, originaltp, positionsource, location) Values (2072923, 44207, 12,ST_GeomFromText('POINT(28.1885 -25.9942)',4326));

select * from positionsource;
insert into positionsource (id, lastmodified, name) values (12, now(), 'ANN');

INSERT into PositionEstimate (id, tp, azimuth, distance, error) Values (44207,44207, 285.4556, 4896.33631, 0); 

delete from testpoint where positionsource>1;
select * from neuralnet;

delete from neuralnet where id>25;
delete from anninputlist where annid>25;
truncate table positionestimate;