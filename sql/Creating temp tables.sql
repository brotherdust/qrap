INSERT INTO filesets values (1, now(),1, ' BryanstonDEM ', 'DEM', null, 5, now(), true, 
false, null, 'GDALFILE', 'UTM', 
'+proj=utm +zone=35 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs');

INSERT INTO sourcefiles (lastmodified, machineid, filesetkey, filename,
location, centmer,areasquare) VALUES (now(), 1, 1, 
'DEM_5m.img', '/home/maggie/GISdata/Bryanston/', 35, 
BOX'((-5.576510192e+193,-8.247067047e+175),(-4.278320654e+200,-7.043908348e+182))');


select * from sourcefiles;

truncate table sourcefiles;
truncate table filesets cascade;

drop table tempsite;

create table tempsite 
(id integer primary key,
lastmodified timestamp,
machineid integer,
sitename text,
status char(30),
longitude decimal,
latitude decimal,
groundheight decimal);

delete from site where id in
(select id from tempsite);



drop table tempcell;

truncate table radioinstallation cascade;

create table tempcell 
(id integer primary key,
lastmodified timestamp,
machineid integer,
mastercell integer,
risector integer,
permanent boolean,
cstraffic decimal,
pstraffic decimal,
txpower decimal,
beacon integer,
layerthreshold decimal,
longitude decimal,
latitude decimal);

truncate table site cascade;

COPY... 

insert into site 
(select id, lastmodified, machineid, sitename, status, groundheight,
ST_SetSRID(ST_MakePoint(Longitude, Latitude),4326)
from tempsite);

truncate table cell cascade;
truncate table tempcell;

insert into cell 
(select id, lastmodified, machineid, mastercell,
risector, permanent, cstraffic, pstraffic, txpower,
beacon, layerthreshold,
ST_SetSRID(ST_MakePoint(Longitude, Latitude),4326)
from tempcell);

truncate table celllist;

Insert into celllist 
(select cell.id, frequencyallocationlist.lastmodified, 
cell.id, frequencyallocationlist.channel, cell.beacon
from frequencyallocationlist cross join cell
where frequencyallocationlist.ci = cell.id
and carrier=0);

delete from frequencyallocationlist where id in
(select f1.id
from frequencyallocationlist as f1 
cross join frequencyallocationlist as f2
where f2.carrier=f1.carrier
and f2.carrier=13
and f1.channel>f2.channel
and f1.ci=f2.ci);

delete from frequencyallocationlist where id in
(select f1.id
from frequencyallocationlist as f1 
cross join frequencyallocationlist as f2
where f2.carrier=13
and f1.channel=f2.channel
and f1.ci=f2.ci
and f1.carrier>f2.carrier);



truncate table frequencyallocationlist;


truncate table testpoint cascade;

SELECT measurement.tp as tp, ST_AsText(testpoint.location) as origLocation, measurement.id as id, siteid, ST_AsText(site.location) as siteLocation, radioinstallation.id as InstKeyFixed, txantennaheight, EIRP, txbearing, txmechtilt, txantpatternkey, azibeamwidth, ci, ST_AsText(centriod) as centriod, measurement.frequency as frequency, measvalue, pathloss, min(testpointauxUTRAN.RxTxDiff) as timeDiff1, min(testpointauxUTRAN.RxTxDiff2) as timeDiff2, min(testpointauxGSM.TA) as TA, technology.DistRes as DistRes from measurement left join testpointauxUTRAN on (measurement.tp=testpointauxUTRAN.tp and measurement.ci=testpointauxUTRAN.servci)left join testpointauxGSM on (measurement.tp=testpointauxGSM.tp and measurement.ci=testpointauxGSM.servci) cross join testpoint cross join cell cross join radioinstallation  cross join antennapattern cross join site  LEFT OUTER JOIN technology ON (radioinstallation.techkey=technology.id) where measurement.tp=testpoint.id and measurement.ci=cell.id and siteid=site.id and cell.risector=radioinstallation.id and radioinstallation.txantpatternkey = antennapattern.id  and site.Location @ ST_GeomFromText('POLYGON((28.29 -25.78,28.29 -25.6,28.15 -25.6,28.15 -25.78,28.29 -25.78))',4326)  group by siteid, measurement.id, InstKeyFixed, azibeamwidth, origLocation, measurement.tp, measurement.ci, siteLocation, centriod, DistRes  order by measurement.tp, timeDiff1, timeDiff2, TA, measvalue;

drop table tempmeaspoint;

create table tempmeaspoint
(TimeD		varchar(50),
MS		varchar(15), 	FrameNumber 	varchar(50), 	Direction	varchar(50), 	
MessageType	varchar(50),	Event		varchar(50),	EventInfo	integer,
Altitude_ft	decimal,		Altitude_m	decimal,		
Latitude		decimal,		Longitude	decimal,		Mode_System	varchar(50),
c1AggrEbIo	decimal,		c1AggrEc		decimal,		c1AggrEcIo	decimal,
c1AggrVSPeakEc	decimal,		c1CellName	varchar(50),	c1CellType	integer,
c1DelaySpread	integer,		c1IntraFreqCells	integer,		c1Io		decimal,
c1NoOfSCs	integer,		c1PeakEbIo	decimal,		c1PeakEc		decimal,
c1PeakEcIo	decimal,		c1PSCH_Ec	decimal,		c1PSCH_EcIo	decimal,
c1RakeCount	integer,		c1SC		integer,		c1SCGroup	integer,
c1SIR		decimal,		c1SSCH_Ec	decimal,		c1SSCH_EcIo	decimal,
c1TimeOffset	integer,		c1TimeOffsetSlotPos	integer,
c1UARFCN		integer,		c1UL_Interference	decimal,
c2AggrEbIo	decimal,		c2AggrEc		decimal,		c2AggrEcIo	decimal,
c2AggrVSPeakEc	decimal,		c2CellName	varchar(50),	c2CellType	integer,
c2DelaySpread	integer,		c2IntraFreqCells	integer,		c2Io		decimal,
c2NoOfSCs	integer,		c2PeakEbIo	decimal,		c2PeakEc		decimal,
c2PeakEcIo	decimal,		c2PSCH_Ec	decimal,		c2PSCH_EcIo	decimal,
c2RakeCount	integer,		c2SC		integer,		c2SCGroup	integer,
c2SIR		decimal,		c2SSCH_Ec	decimal,		c2SSCH_EcIo	decimal,
c2TimeOffset	integer,		c2TimeOffsetSlotPos	integer,
c2UARFCN		integer,		c2UL_Interference	decimal,
c3AggrEbIo	decimal,		c3AggrEc		decimal,		c3AggrEcIo	decimal,
c3AggrVSPeakEc	decimal,		c3CellName	varchar(50),	c3CellType	integer,
c3DelaySpread	integer,		c3IntraFreqCells	integer,		c3Io		decimal,
c3NoOfSCs	integer,		c3PeakEbIo	decimal,		c3PeakEc		decimal,
c3PeakEcIo	decimal,		c3PSCH_Ec	decimal,		c3PSCH_EcIo	decimal,
c3RakeCount	integer,		c3SC		integer,		c3SCGroup	integer,
c3SIR		decimal,		c3SSCH_Ec	decimal,		c3SSCH_EcIo	decimal,
c3TimeOffset	integer,		c3TimeOffsetSlotPos	integer,
c3UARFCN		integer,		c3UL_Interference	decimal,
Speed_km_h	decimal,		Speed_m_h	decimal,		TTime		varchar(50),
BAggrEbIo	decimal,		BAggrEc		decimal,		BAggrEcIo	decimal,
BAggrVSPeakEc	decimal,		BCellName	varchar(50),	BCellType	integer,
BDelaySpread	integer,						BIo		decimal,
BNoOfSCs		integer,		BPeakEbIo	decimal,		BPeakEc		decimal,
BPeakEcIo	decimal,		BPSCH_Ec		decimal,		BPSCH_EcIo	decimal,
BRakeCount	integer,		BSC		integer,		BSCGroup		integer,
BSIR		decimal,		BSSCH_Ec		decimal,		BSSCH_EcIo	decimal,
BTimeOffset	integer,		BTimeOffsetSlotPos	integer,
BUARFCN		integer
);

create table tempmeaspointID
(ID bigserial primary key unique not null,
TimeD		varchar(50),
MS		varchar(15), 	FrameNumber 	varchar(50), 	Direction	varchar(50), 	
MessageType	varchar(50),	Event		varchar(50),	EventInfo	integer,
Altitude_ft	decimal,		Altitude_m	decimal,		
Latitude		decimal,		Longitude	decimal,		Mode_System	varchar(50),
c1AggrEbIo	decimal,		c1AggrEc		decimal,		c1AggrEcIo	decimal,
c1AggrVSPeakEc	decimal,		c1CellName	varchar(50),	c1CellType	integer,
c1DelaySpread	integer,		c1IntraFreqCells	integer,		c1Io		decimal,
c1NoOfSCs	integer,		c1PeakEbIo	decimal,		c1PeakEc		decimal,
c1PeakEcIo	decimal,		c1PSCH_Ec	decimal,		c1PSCH_EcIo	decimal,
c1RakeCount	integer,		c1SC		integer,		c1SCGroup	integer,
c1SIR		decimal,		c1SSCH_Ec	decimal,		c1SSCH_EcIo	decimal,
c1TimeOffset	integer,		c1TimeOffsetSlotPos	integer,
c1UARFCN		integer,		c1UL_Interference	decimal,
c2AggrEbIo	decimal,		c2AggrEc		decimal,		c2AggrEcIo	decimal,
c2AggrVSPeakEc	decimal,		c2CellName	varchar(50),	c2CellType	integer,
c2DelaySpread	integer,		c2IntraFreqCells	integer,		c2Io		decimal,
c2NoOfSCs	integer,		c2PeakEbIo	decimal,		c2PeakEc		decimal,
c2PeakEcIo	decimal,		c2PSCH_Ec	decimal,		c2PSCH_EcIo	decimal,
c2RakeCount	integer,		c2SC		integer,		c2SCGroup	integer,
c2SIR		decimal,		c2SSCH_Ec	decimal,		c2SSCH_EcIo	decimal,
c2TimeOffset	integer,		c2TimeOffsetSlotPos	integer,
c2UARFCN		integer,		c2UL_Interference	decimal,
c3AggrEbIo	decimal,		c3AggrEc		decimal,		c3AggrEcIo	decimal,
c3AggrVSPeakEc	decimal,		c3CellName	varchar(50),	c3CellType	integer,
c3DelaySpread	integer,		c3IntraFreqCells	integer,		c3Io		decimal,
c3NoOfSCs	integer,		c3PeakEbIo	decimal,		c3PeakEc		decimal,
c3PeakEcIo	decimal,		c3PSCH_Ec	decimal,		c3PSCH_EcIo	decimal,
c3RakeCount	integer,		c3SC		integer,		c3SCGroup	integer,
c3SIR		decimal,		c3SSCH_Ec	decimal,		c3SSCH_EcIo	decimal,
c3TimeOffset	integer,		c3TimeOffsetSlotPos	integer,
c3UARFCN		integer,		c3UL_Interference	decimal,
Speed_km_h	decimal,		Speed_m_h	decimal,		TTime		varchar(50),
BAggrEbIo	decimal,		BAggrEc		decimal,		BAggrEcIo	decimal,
BAggrVSPeakEc	decimal,		BCellName	varchar(50),	BCellType	integer,
BDelaySpread	integer,						BIo		decimal,
BNoOfSCs		integer,		BPeakEbIo	decimal,		BPeakEc		decimal,
BPeakEcIo	decimal,		BPSCH_Ec		decimal,		BPSCH_EcIo	decimal,
BRakeCount	integer,		BSC		integer,		BSCGroup		integer,
BSIR		decimal,		BSSCH_Ec		decimal,		BSSCH_EcIo	decimal,
BTimeOffset	integer,		BTimeOffsetSlotPos	integer,
BUARFCN		integer
);

insert into tempmeaspointID 
(TimeD,	MS, 	FrameNumber, 	Direction, 	MessageType,	Event,	EventInfo,
Altitude_ft,	Altitude_m,	Latitude,	Longitude,	Mode_System,
c1AggrEbIo,	c1AggrEc,	c1AggrEcIo,	c1AggrVSPeakEc,	c1CellName,	
c1CellType,	c1DelaySpread,	c1IntraFreqCells,	
c1Io,		c1NoOfSCs,	c1PeakEbIo,	c1PeakEc,	c1PeakEcIo,
c1PSCH_Ec,	c1PSCH_EcIo,	c1RakeCount,	c1SC,		c1SCGroup,
c1SIR,		c1SSCH_Ec,	c1SSCH_EcIo,	c1TimeOffset,	c1TimeOffsetSlotPos,
c1UARFCN,	c1UL_Interference,
c2AggrEbIo,	c2AggrEc,	c2AggrEcIo,	c2AggrVSPeakEc,	c2CellName,	
c2CellType,	c2DelaySpread,	c2IntraFreqCells,	
c2Io,		c2NoOfSCs,	c2PeakEbIo,	c2PeakEc,	c2PeakEcIo,
c2PSCH_Ec,	c2PSCH_EcIo,	c2RakeCount,	c2SC,		c2SCGroup,
c2SIR,		c2SSCH_Ec,	c2SSCH_EcIo,	c2TimeOffset,	c2TimeOffsetSlotPos,
c2UARFCN,	c2UL_Interference,
c3AggrEbIo,	c3AggrEc,	c3AggrEcIo,	c3AggrVSPeakEc,	c3CellName,	
c3CellType,	c3DelaySpread,	c3IntraFreqCells,	
c3Io,		c3NoOfSCs,	c3PeakEbIo,	c3PeakEc,	c3PeakEcIo,
c3PSCH_Ec,	c3PSCH_EcIo,	c3RakeCount,	c3SC,		c3SCGroup,
c3SIR,		c3SSCH_Ec,	c3SSCH_EcIo,	c3TimeOffset,	c3TimeOffsetSlotPos,
c3UARFCN,	c3UL_Interference,
Speed_km_h,	Speed_m_h,	TTime,
BAggrEbIo,	BAggrEc,		BAggrEcIo,	BAggrVSPeakEc,	BCellName,	
BCellType,	BDelaySpread,		
BIo,		BNoOfSCs,	BPeakEbIo,	BPeakEc,		BPeakEcIo,
BPSCH_Ec,	BPSCH_EcIo,	BRakeCount,	BSC,		BSCGroup,
BSIR,		BSSCH_Ec,	BSSCH_EcIo,	BTimeOffset,	BTimeOffsetSlotPos,
BUARFCN) select * from tempmeaspoint;


delete from tempmeaspoint where BUARFCN is null;

select count(*) from tempmeaspoint;

insert into testpoint 
(select ID, now(), null, 
2, 1, 1, Altitude_m,
ST_SetSRID(ST_MakePoint(Longitude, Latitude),4326)
from tempmeaspointID);

create table tempmeas
(ID bigserial primary key unique not null,
TP bigint references testpoint(id),
CI integer references cell(id),
Distance		decimal,
AggrEbIo		decimal,		AggrEc		decimal,		AggrEcIo		decimal,
AggrVSPeakEc	decimal,		CellName		varchar(50),	CellType		integer,
DelaySpread	integer,		IntraFreqCells	integer,		Io		decimal,
NoOfSCs		integer,		PeakEbIo		decimal,		PeakEc		decimal,
PeakEcIo		decimal,		PSCH_Ec		decimal,		PSCH_EcIo	decimal,
RakeCount	integer,		SC		integer,		SCGroup		integer,
SIR		decimal,		SSCH_Ec		decimal,		SSCH_EcIo	decimal,
TimeOffset	integer,		TimeOffsetSlotPos	integer,
UARFCN		integer);


insert into measurement
select id, ci, tp, UARFCN*0.2, null, null, Distance, null, aggrEC, null
from tempmeas;

insert into tempmeas 
(TP,CI,Distance,
AggrEbIo	,	AggrEc,		AggrEcIo,	AggrVSPeakEc,	CellName	,	CellType	,
DelaySpread,	IntraFreqCells,	Io,		NoOfSCs,		PeakEbIo	,	PeakEc,
PeakEcIo	,	PSCH_Ec,		PSCH_EcIo,	RakeCount,	SC,		SCGroup,
SIR,		SSCH_Ec,		SSCH_EcIo,	TimeOffset,	TimeOffsetSlotPos,
UARFCN)
select testpoint.id, cell.id, ST_distance(cell.centriod, testpoint.location) as Distance,
c1AggrEbIo,	c1AggrEc,	c1AggrEcIo,	c1AggrVSPeakEc,	c1CellName,	
c1CellType,	c1DelaySpread,	c1IntraFreqCells,	
c1Io,		c1NoOfSCs,	c1PeakEbIo,	c1PeakEc,	c1PeakEcIo,
c1PSCH_Ec,	c1PSCH_EcIo,	c1RakeCount,	c1SC,		c1SCGroup,
c1SIR,		c1SSCH_Ec,	c1SSCH_EcIo,	c1TimeOffset,	c1TimeOffsetSlotPos,
c1UARFCN
from tempmeaspointid left outer join celllist on (c1UARFCN = Carrier) and (c1SC = Code)
left outer join cell on (ci=cell.id)
left outer join testpoint on (tempmeaspointid.id = testpoint.id)
where c1UARFCN is not null;


delete from tempmeas where id in
(select t2.id from 
tempmeas as t1 cross join tempmeas as t2
where t1.tp = t2.tp
and t1.SC = t2.SC
and t1.SCGroup = t2.SCGroup
and t1.UARFCN = t2.UARFCN
and t2.Distance > t1.Distance
and t1.ci is not null);

delete from tempmeas where id in
(select t2.id from 
tempmeas as t1 cross join tempmeas as t2
where t1.tp = t2.tp
and t1.SC = t2.SC
and t1.SCGroup = t2.SCGroup
and t1.UARFCN = t2.UARFCN
and t2.CI is null
and t1.id < t2.id);

delete from tempmeas where Ci is null;

select max(id) from tempmeas;
where ci is null;

select * from tempmeas
where id<1000;

drop table unfound;

create table unfound as
(select sc, scgroup, uarfcn
from tempmeas
where ci is null);

select count(*) from tempmeas;

drop table tempmeas;

Drop table NotFound;

create table NotFound as
select sc, scgroup, uarfcn, psch_ec, location
from tempmeas cross join testpoint
where tempmeas.tp=testpoint.id
and psch_ec>-100
and tempmeas.ci is null;

select count(*) from 
tempmeas where sc=146 and uarfcn=10737;

select * from tempmeas where id<1000;