Drop table RawGSMTEMS;

Create table RawGSMTEMS
(MeasTime	varchar(20),	MS		varchar(15),	FrameNumber	varchar(30),
Direction	varchar(15),	MessageType	varchar(70),	Event		varchar(70),
EventInfo	varchar(250),	BCCH		integer,		TCH		integer,
BSIC_		varchar(10),	BSIC_Num		integer,		C1		integer,
C2		integer,		C31		integer,		C32		integer,
CellId		integer,		CGI		varchar(30),	DTXRateDL	integer,
FBand		varchar(15),	Fband_Num	integer,		Latitude		real,
LatitudeDD_Text	real,		Longitude	real,		LongitudeDD_Text	real,
MSTxPower_dBm	integer,		MSPowerControlLevel integer,	Mode		varchar(15),
NeighborARFCN_1	integer,		NeighborBSIC_1	varchar(10),	Neighbor_BSIC_Num_1 integer,
NeighborC1_1	integer,		NeighborC2_1	integer,		NeighborC31_1	integer,
Neighbor_C32_1	integer,		NeighborCellId_1	integer,		NeighborRxLev_dBm_1 integer,
RxLevFull	integer,		RxLevFull_dBm	integer,		RxLevSub		integer,
RxLevSub_dBm	integer,		RxQualFull	integer,		RxQualSub	integer,
TA		integer,		TDSCDMANeighborNoOf integer,	Time2		varchar(20),
WCDMANeighborSC_1 integer,	WCDMANeighborUARFCN_1 integer,	WCDMANeighborsNoOf integer,
WCDMANeighborCPICH_RSCP_dBm_1 	integer,		
NeighborARFCN_2	integer,		NeighborBSIC_2	varchar(10),	NeighborBSIC_Num_2 integer,
NeighborC1_2	integer,		NeighborC2_2	integer,		NeighborC31_2	integer,
NeighborC32_2	integer,		NeighborCellId_2	integer,		NeighborRxLev_1	integer,
NeighborRxLev_dBm_2 	integer,
NeighborARFCN_3	integer,		NeighborBSIC_3	varchar(10),	NeighborBSIC_Num_3 integer,
NeighborC1_3	integer,		NeighborC2_3	integer,		NeighborC31_3	integer,
NeighborC32_3	integer,		NeighborCellId_3	integer,		NeighborRxLev_2	integer,
NeighborRxLev_dBm_3	integer,
NeighborARFCN_4	integer,		NeighborBSIC_4	varchar(10),	NeighborBSIC_Num_4 integer,
NeighborC1_4	integer,		NeighborC2_4	integer,		NeighborC31_4	integer,
NeighborC32_4	integer,		NeighborCellId_4	integer,		NeighborRxLev_3	integer,
NeighborRxLev_dBm_4	integer,
NeighborARFCN_5	integer,		NeighborBSIC_5	varchar(10),	NeighborBSIC_Num_5 integer,
NeighborC1_5	integer,		NeighborC2_5	integer,		NeighborC31_5	integer,
NeighborC32_5	integer,		NeighborCellId_5	integer,		NeighborRxLev_4	integer,
NeighborRxLev_dBm_5	integer,
NeighborARFCN_6	integer,		NeighborBSIC_6	varchar(10),	NeighborBSIC_Num_6 integer,
NeighborC1_6	integer,		NeighborC2_6	integer,		NeighborC31_6	integer,
NeighborC32_6	integer,		NeighborCellId_6	integer,		NeighborRxLev_5	integer,
NeighborRxLev_dBm_6 integer);

select * from RawGSMTEMS;

drop table tempRawGSMTEMS;
create table tempRawGSMTEMS
(ID bigserial primary key unique not null,
MeasTime	varchar(20),	MS		varchar(15),	FrameNumber	varchar(30),
Direction	varchar(15),	MessageType	varchar(70),	Event		varchar(70),
EventInfo	varchar(250),	BCCH		integer,		TCH		integer,
BSIC_		varchar(10),	BSIC_Num		integer,		C1		integer,
C2		integer,		C31		integer,		C32		integer,
CellId		integer,		CGI		varchar(30),	DTXRateDL	integer,
FBand		varchar(15),	Fband_Num	integer,		Latitude		real,
LatitudeDD_Text	real,		Longitude	real,		LongitudeDD_Text	real,
MSTxPower_dBm	integer,		MSPowerControlLevel integer,	Mode		varchar(15),
NeighborARFCN_1	integer,		NeighborBSIC_1	varchar(10),	Neighbor_BSIC_Num_1 integer,
NeighborC1_1	integer,		NeighborC2_1	integer,		NeighborC31_1	integer,
Neighbor_C32_1	integer,		NeighborCellId_1	integer,		NeighborRxLev_dBm_1 integer,
RxLevFull	integer,		RxLevFull_dBm	integer,		RxLevSub		integer,
RxLevSub_dBm	integer,		RxQualFull	integer,		RxQualSub	integer,
TA		integer,		TDSCDMANeighborNoOf integer,	Time2		varchar(20),
WCDMANeighborSC_1 integer,	WCDMANeighborUARFCN_1 integer,	WCDMANeighborsNoOf integer,
WCDMANeighborCPICH_RSCP_dBm_1 	integer,		
NeighborARFCN_2	integer,		NeighborBSIC_2	varchar(10),	NeighborBSIC_Num_2 integer,
NeighborC1_2	integer,		NeighborC2_2	integer,		NeighborC31_2	integer,
NeighborC32_2	integer,		NeighborCellId_2	integer,		NeighborRxLev_1	integer,
NeighborRxLev_dBm_2 	integer,
NeighborARFCN_3	integer,		NeighborBSIC_3	varchar(10),	NeighborBSIC_Num_3 integer,
NeighborC1_3	integer,		NeighborC2_3	integer,		NeighborC31_3	integer,
NeighborC32_3	integer,		NeighborCellId_3	integer,		NeighborRxLev_2	integer,
NeighborRxLev_dBm_3	integer,
NeighborARFCN_4	integer,		NeighborBSIC_4	varchar(10),	NeighborBSIC_Num_4 integer,
NeighborC1_4	integer,		NeighborC2_4	integer,		NeighborC31_4	integer,
NeighborC32_4	integer,		NeighborCellId_4	integer,		NeighborRxLev_3	integer,
NeighborRxLev_dBm_4	integer,
NeighborARFCN_5	integer,		NeighborBSIC_5	varchar(10),	NeighborBSIC_Num_5 integer,
NeighborC1_5	integer,		NeighborC2_5	integer,		NeighborC31_5	integer,
NeighborC32_5	integer,		NeighborCellId_5	integer,		NeighborRxLev_4	integer,
NeighborRxLev_dBm_5	integer,
NeighborARFCN_6	integer,		NeighborBSIC_6	varchar(10),	NeighborBSIC_Num_6 integer,
NeighborC1_6	integer,		NeighborC2_6	integer,		NeighborC31_6	integer,
NeighborC32_6	integer,		NeighborCellId_6	integer,		NeighborRxLev_5	integer,
NeighborRxLev_dBm_6 integer);

insert into tempRawGSMTEMS
(MeasTime,	MS,	FrameNumber,
Direction,	MessageType,	Event,
EventInfo,	BCCH,		TCH,
BSIC_,	BSIC_Num	,		C1,
C2,		C31,		C32,
CellId,		CGI,	DTXRateDL,
FBand,	Fband_Num,		Latitude,
LatitudeDD_Text,	Longitude,		LongitudeDD_Text,
MSTxPower_dBm,		MSPowerControlLevel,	Mode,
NeighborARFCN_1,		NeighborBSIC_1,	Neighbor_BSIC_Num_1,
NeighborC1_1,		NeighborC2_1,		NeighborC31_1,
Neighbor_C32_1,		NeighborCellId_1,		NeighborRxLev_dBm_1,
RxLevFull,		RxLevFull_dBm,		RxLevSub	,
RxLevSub_dBm,		RxQualFull,		RxQualSub,
TA,		TDSCDMANeighborNoOf,	Time2,
WCDMANeighborSC_1,	WCDMANeighborUARFCN_1,	WCDMANeighborsNoOf,
WCDMANeighborCPICH_RSCP_dBm_1,		
NeighborARFCN_2,		NeighborBSIC_2,	NeighborBSIC_Num_2,
NeighborC1_2,		NeighborC2_2,		NeighborC31_2,
NeighborC32_2,		NeighborCellId_2,		NeighborRxLev_1,
NeighborRxLev_dBm_2,
NeighborARFCN_3,		NeighborBSIC_3,	NeighborBSIC_Num_3,
NeighborC1_3,		NeighborC2_3,		NeighborC31_3,
NeighborC32_3,		NeighborCellId_3,		NeighborRxLev_2,
NeighborRxLev_dBm_3,
NeighborARFCN_4,		NeighborBSIC_4,	NeighborBSIC_Num_4,
NeighborC1_4,		NeighborC2_4,		NeighborC31_4,
NeighborC32_4,		NeighborCellId_4,		NeighborRxLev_3,
NeighborRxLev_dBm_4,
NeighborARFCN_5,		NeighborBSIC_5,	NeighborBSIC_Num_5,
NeighborC1_5,		NeighborC2_5,		NeighborC31_5,
NeighborC32_5,		NeighborCellId_5,		NeighborRxLev_4,
NeighborRxLev_dBm_5,
NeighborARFCN_6,		NeighborBSIC_6,	NeighborBSIC_Num_6,
NeighborC1_6,		NeighborC2_6,		NeighborC31_6,
NeighborC32_6,		NeighborCellId_6,		NeighborRxLev_5,
NeighborRxLev_dBm_6)
select * from RawGSMTEMS;

truncate table tempRawGSMTEMS;

select min(id) from tempRawGSMTEMS;

select setval(ID, 1)
from tempRawGSMTEMS;

delete from tempRawGSMTEMS
where CellID is null
and BSIC_num is null;

truncate table toDeleteTPlist;

delete from tempRawGSMTEMS
where id in
(select tp from toDeleteTPlist);

select count(*) from tempRawGSMTEMS;

drop table tpUsed;

create table tpUsed as
(select tp from testpointauxGSM
cross join cell
cross join radioinstallation
where servci=cell.id
and risector = radioinstallation.id
and siteid in
(select siteid from sitelist));

select * from sitelist;

drop table GSMtempmeas;
create table GSMtempmeas
(ID bigserial primary key unique not null,
tp bigint,
BCCH integer,
BSIC integer,
CI integer,
RxLev real);

select setval(ID, 1)
from GSMtempmeas;
select count(*)/48 from anninputlist;

truncate table positionestimate cascade;

select * from positionsource;

insert into positionsource values
(13, now(),1, 'Combined', 'Using ANN direction, but do line search on distance');

insert into testpoint
(id, timeofmeas, location, measdatasource, positionsource, meastype)
select tempRawGSMTEMS.id as id, now(), ST_SetSRID(ST_MakePoint(Longitudedd_text, Latitudedd_text),4326), 6, 1, 2
from tempRawGSMTEMS cross join tpUsed
where id = tpUsed.tp;

select count(*) from tempRawGSMTEMS;
select count(*) from tempGSMaux;

delete from testpointauxGSM where id in
(select tp from train);


INSERT into testpoint (id, originaltp, positionsource, location) Values (2072233, 52849, 13,ST_GeomFromText('POINT(28.1705 -26.0492)',4326));


truncate table positionestimate;

select count(*) from neuralnet;

create table tpUsed as
select tp from testpointauxGSM;

truncate table tpUsed;

select count(*) from tpUsed;

delete from tpUsed
where tp in
(select tp from 

delete from testpoint
where id in
(select tp from train);

select count(*) from temptestpoint;


select count(*) from tempGSMaux;

truncate table tempGSMaux;

insert into tempGSMaux
(select id, id as tp, CellID as servci, TA, 
mstxpower_dbm as mstxpower, rxlevsub_dbm as MSrxpower,
rxQualSub as MSrxQual, BCCH, bsic_num as BSIC, 
ST_SetSRID(ST_MakePoint(Longitudedd_text, Latitudedd_text),4326) as location
from tempRawGSMTEMS
where rxlevsub_dBm>=rxlevFull_dBm
and rxlevsub_dBm is not null
and ((bsic_num is not null) or (cellID is not null))
and ((bcch is not null) or (cellID is not null)));

select count(*) from testpointauxGSM;

truncate table testpointauxGSM;

insert into testpointauxGSM
(id, tp, servci, TA, MStxPower, MSrxPower, MSrxQual)
select id, tempGSMaux.tp as tp, servci, TA, MStxPower, MSrxPower,MSrxQual
from tempGSMaux cross join tpUsed
where tempGSMaux.tp=tpUsed.tp;

create table toDeleteTPList
(tp bigint);

select count(*) from toDeleteTPlist;

truncate table testpoint cascade;

insert into testpoint
(id, timeofmeas, location, measdatasource, positionsource, meastype)
select tempRawGSMTEMS.id as id, now(), ST_SetSRID(ST_MakePoint(Longitudedd_text, Latitudedd_text),4326), 6, 1, 2
from tempRawGSMTEMS cross join tpUsed
where id = tpUsed.tp;

delete 

 

delete from testpoint where id in
(select tpt from testlist);

drop table testlist;

create table testlist as
select tp from tpSequence
where sq%8=0;

select * from test;

select count(*) from trainlist;

drop table trainlist;

select count(*) from measurement;

create table trainlist as 
select tp from tpUsed where 
tp not in (select tp from testlist);

drop table temptestpoint;

select count(*) from tpUsed;

delete from testpointauxGSM 
where tp not in
(select tp from tpUsed);

create table alles as select * from train;
insert into alles select * from test;

truncate table test;

truncate table neuralnet;

insert into train select * from alles
where tp in (select tp from trainlist);

drop table temptestpoint;
create table tempmeasurement as select * from measurement where tp in (select tp from tpused);

truncate table testpoint cascade;

select * from testpoint;

truncate table neuralnet;
truncate table anninputlist;

insert into testpoint 
select id, timeofmeas, originaltp, measdatasource, positionsource, meastype, height, location
from temptestpoint cross join testlist
where tpt = id;


delete from testpoint
where id not in 
(select tp from testpointauxGSM);

truncate table measurement;

select count(*) from testpoint;

select count(*) from tplist;

drop table test;

create table test as select
distinct testpointauxGSM.tp as tp, servci, ta
from testpointauxGSM cross join testlist
where testpointauxGSM.tp = testlist.tp;

drop table train;

create table train as select
distinct testpointauxGSM.tp as tp, servci, ta
from testpointauxGSM cross join trainlist
where testpointauxGSM.tp = trainlist.tp;

delete from positionestimate where tp in
(select id from testpoint where positionsource>1);

select * from positionestimate;

select count(*) from trainlist;

truncate table neuralnet cascade;

create table testpointauxGSMTest as
select * from testpointauxGSM 
where tp in
(select tpt from testlist);

select * from testpoint;

drop table tpused;

create table tpused as select tp from testpointauxGSM;

drop table TPsequence;

create table TPsequence
(sq bigserial primary key unique not null,
tp bigint);

insert into TPsequence (tp)
select tp from tpUsed
order by tp;

drop table Temptestpoint;

create TABLE  Temptestpoint as
select sq, testpoint.*, 
testpointauxGSM.servci as servci, testpointauxGSM.ta as ta
from testpoint cross join TPsequence cross join tpUsed
cross join testpointauxGSM
where testpoint.id=TPsequence.tp
and testpoint.id=testpointauxGSM.tp
and testpoint.id=tpUsed.tp;

drop table tpDistance;

create table tpDistance as
(select t0.id as tp0, t1.id as tp1, 
t0.sq as sq0, t1.sq as sq1, 
m0.num as num0, m1.num as num1,
t0.servci as ci0, t1.servci as ci1, 
t0.ta as ta0, t1.ta as ta1, 
distance(t0.location, t1.location) as distance
from Temptestpoint as t0
cross join Temptestpoint as t1
cross join MeasPerTP as m0
cross join MeasPerTP as m1
where m0.tp=t0.id
and m1.tp=t1.id
and t0.sq=t1.sq+1);

delete from tpUsed where tp in
(select tp0 as tp from tpDistance
where distance<3.5e-5
and sq0%2=1
and distance is not null
and num0<=num1);

select count(*) from tpused;

delete from testpointauxGSM where tp 
not in (select tp from tpused);

select min(distance) from tpDistance;

insert into toDeleteTPlist
select tp0 as tp from tpDistance
where distance<1e-4
and sq0%2=0
and distance is not null
and num0<=num1
and ta0=ta1
and ci0=ci1;

select count(*) from cell;

drop table todeletelist;

insert into todeletelist 
(select originaltp as tp from
(select originaltp, min(error) as kleinste from
positionestimate cross join testpoint
where tp=testpoint.id
group by originaltp
order by kleinste desc) as foo
where kleinste>1500);

select count(*) from allcurrent;

truncate table testpoint;

select count(*) from positionestimate;

select count(*) from testpointauxGSM;
INSERT into PositionEstimate (id, tp, azimuth, distance, error) Values (7559,7559, 282.917324, 1610.4777, 0); 

delete from tpused where tp in
(select tp from todeletelist);

insert into todeletelist 
(select tp from testpointauxGSM);

select count(*)
from testpointauxGSM;

create table tempmeas as
(select * from measurement);

truncate table measurement;

insert into measurement
(select * from tempmeas
where tp in 
(select tp from tpused));

drop table tpused;

create table tpused as
(select distinct tp from measurement);

truncate table testpointauxGSM;

insert into testpointauxGSM
(select * from temptestaux
where tp in 
(select tp from tpused));

create table temptestaux as
(select * from testpointauxGSM);



delete from measurement where tp not in
(select tp from tpUsed);

create table todelete 
as select tp from testpointauxGSM
where tp not in (select tp from tpUsed);



create table tpUsed as
select id as tp from testpointauxGSM;

drop table DeleteTPlist;

create table DeleteTPlist
as select distinct tp 
from toDeleteTPlist;

delete from tempTestpoint
where id in
(select tp from DeleteTPlist);

delete from tempRawGSMTEMS
where id in
(select tp from DeleteTPlist);

select count(*) from TempRawGSMTEMS;

delete from testpointauxGSM 
where tp in 
(select tp from toDeleteTPlist);

select avg(NumNB) from
(select siteid, (count(ci)) as NumNB from
(select siteid, ci, count(measurement.id) as num
from radioinstallation 
cross join cell
cross join testpointauxGSM
cross join measurement
where measurement.tp=testpointauxGSM.tp
and testpointauxGSM.servci=cell.id
and cell.risector = radioinstallation.id
and measvalue>-85
group by siteid, ci
order by siteid, num desc) as teller
group by siteid
order by NumNB desc);
 as summer;



drop table tempAuxR; 

create table tempAuxR as
(select * from tempGSMaux 
where servci is null);

delete from tempRawGSMTEMS where id in
(select tp from DeleteTPlist);

select * from tempRawGSMTEMS;

truncate table GSMtempmeas;

insert into GSMtempmeas
(tp, BCCH, BSIC, CI, RxLev)
select ID, TCH, BSIC_Num, CellId, RxLevsub_dBm
from tempRawGSMTEMS
where ((bsic_num is not null) or (cellid is not null))
and ((bcch is not null) or (cellid is not null))
and RxLevsub_dBm is not null
and RxLevSub_dBm>=RxLevFull_dBm
and TCH is not null;

insert into GSMtempmeas
(tp, BCCH, BSIC, CI, RxLev)
select ID, NeighborARFCN_6, NeighborBSIC_Num_6, NeighborCellId_6, NeighborRxLev_dBm_6
from tempRawGSMTEMS
where ((NeighborBSIC_Num_6 is not null) or (NeighborCellId_6 is not null))
and NeighborRxLev_dBm_6 is not null
and NeighborARFCN_6 is not null; 

truncate table measurement;

insert into measurement
(id, ci, tp, frequency, measvalue)
select id, ci, tp, (945+0.2*BCCH) as frequency, rxlev as measvalue 
from GSMtempmeas
where BCCH<512
and tp in (select tp from tpUsed);

insert into measurement
(id, ci, tp, frequency, measvalue)
select id, ci, tp, (1710.2+0.2*(BCCH-512)) as frequency, rxlev as measvalue 
from GSMtempmeas
where BCCH>511
and tp in (select tp from tpUsed);

truncate table testpoint;

select count(*) from GSMtempmeas;

select * from measdatasource;
insert into measdatasource 
select 6, now(), 1, now(), 'GSM TEMS Tembisa','GSM TEMS tembisa', null; 

select * from meastype;
insert into meastype
select 2, now(), 1, 'TEMS active mode';

drop table foundCells;

create table foundCells as
(Select distinct CI, BCCH, BSIC
from GSMtempmeas
where CI is not null
and BCCH is not null
and BSIC is not null);

select * from celllist;

drop table CellsFoundButNoLocation;

create table CellsFoundButNoLocation as
(select CI, BCCH, BSIC 
from foundCells where CI not in
(select id from cell));

select * from neuralnet where id < 24;

select * from anninputlist;

delete from anninputlist where annid < 24;

drop table tempMTR;
 
create table tempMTR as
(select GSMtempmeas.id as id, tp, BCCH,BSIC, CI, location
from GSMtempmeas cross join testpoint
where tp=testPoint.id
and CI is null
and BSIC is not null
and BCCH is not null);

delete from GSMtempmeas where tp in
(select tp from DeleteTPlist);

select count(*) from tempMTR;

delete from GSMtempmeas
where CI in 
(select CI from CellsFoundButNoLocation);

delete from GSMtempmeas
where ci is null 
and (bcch, bsic) in 
(select bcch, bsic from
CellsFoundButNoLocation);

insert into toDeleteTPlist
select tp from tempGSMaux 
where servci is null 
and (bcch, bsic) in 
(select bcch, bsic from
CellsFoundButNoLocation);

delete from tempGSMaux
where servci in
(select CI from 
CellsFoundButNoLocation);
truncate table toDeleteTPlist;

insert into toDeleteTPlist
select tp from tempGSMaux 
where servci in 
(select ci from
CellsFoundButNoLocation);

delete from tempGSMaux 
where servci in 
(select ci from
CellsFoundButNoLocation);

insert into toDeleteTPlist
select tp from tempGSMaux 
where servci is null; 

drop table CellsList;

create table CellsList as
(select CI, BCCH, BSIC, location from
foundCells cross join cell
cross join radioinstallation 
cross join site
where siteid=site.id
and risector = radioinstallation.id
and CI = Cell.id);

delete from CellsList 
where (bsic, bcch) in
(select bsic, bcch from CellsFoundButNoLocation);

select * from CellsFoundButNoLocation;

delete from CellsList 
where ci in
(select distinct cell.id as ci
from cell cross join radioinstallation
where cell.id in 
(select ci from CellsList)
and cell.risector=radioinstallation.id
and radioinstallation.id-siteid*10 >4);


drop table Combination;

create table Combination as
(select tempAuxR.id as id, tp, CellsList.bcch as bcch, CellsList.bsic as bsic, 
CellsList.ci as ci, distance(tempAuxR.location, CellsList.location) as afstand
from tempAuxR cross join CellsList
where tempAuxR.bcch=CellsList.bcch
and tempAuxR.bsic=CellsList.bsic
order by tp, id, afstand);

drop table CombinationM;

create table CombinationM as
(select tempMTR.id as id, tp, CellsList.bcch as bcch, CellsList.bsic as bsic, 
CellsList.ci as ci, distance(tempMTR.location, CellsList.location) as afstand
from tempMTR cross join CellsList
where tempMTR.bcch=CellsList.bcch
and tempMTR.bsic=CellsList.bsic
order by tp, id, afstand);

delete from Combination where (id, ci) in
(select c1.id, c1.ci from
Combination as c1 
cross join Combination as c2
where c1.id=c2.id
and c1.afstand>c2.afstand); 

delete from CombinationM where (id, ci) in
(select c1.id, c1.ci from
CombinationM as c1 
cross join CombinationM as c2
where c1.id=c2.id
and c1.afstand>c2.afstand); 

drop table duplicates;

create table duplicates as
(select id, count(*) as num 
from CombinationM
group by id
order by num); 

delete from CombinationM where id in
(select id from duplicates
where num>1);

select count(*) from duplicates where num>1;

update tempGSMaux set servci = Combination.CI from Combination
where tempGSMaux.Id = Combination.ID;

delete from tempGSMaux 
where servci is null;
where tp in
(select tp from toDeleteTPlist);

delete from GSMtempmeas
where tp in
(select tp from toDeleteTPlist);


update GSMtempmeas set ci = CombinationM.CI from CombinationM
where GSMtempmeas.Id = CombinationM.ID;

delete from GSMtempmeas
where CI is null;

drop table MeasPerTP;

create table MeasPerTP as
select tp, count(*) as num
from measurement
group by tp;

select count(tp)
from MeasPerTP 
where num<2;

delete from GSMtempmeas 
where ci in 
(select ci from CellsFoundButNoLocation);

create table TCHlist
(CI integer,
TCH1 integer,
TCH2 integer,
TCH3 integer,
TCH4 integer,
TCH5 integer,
TCH6 integer,
TCH7 integer,
TCH8 integer,
TCH9 integer,
TCH10 integer,
TCH11 integer,
TCH12 integer);



insert into frequencyallocationlist
(lastmodified, machineid, ci, carrier, channel)
select now(),1,ci,0,BCCH
from CellsList
where BCCH is not null 
and ci not in
(select ci from frequencyallocationlist
where carrier=0);

drop table tpUsed;
create table tpUsed
as select distinct tp from GSMtempmeas
order by tp;

create table testlist 

select ta, count(*) 
from testpoint cross join testpointauxGSM
where testpoint.id=tp
group by ta
order by ta;

delete from testpoint where id in 
(select tp from train);

