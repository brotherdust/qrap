update testpoint set measdatasource=1;

drop table allimport;

create table allImport(
EMG	char(5),	CellID		char(20),	
CGI	char(20),	SiteID		integer,
Longitude	double precision,	Latitude	double precision,
TECHNOLOGY	char(5),	CELL_LAYER	char(10),	Band		char(10),
AntennaPattern	char(50),	AntennaHeight	real,		AntMechTilt	real,
Azimuth		real,		TCELLID		char(20),	EIRP		real);

create table TempCells as
(select  
regexp_matches(substring (CGI from 8 for (char_length(CGI)-7)), '(.*)(\-)(.*)') as LACCI,
(ascii(substring(CellID from (char_length(CellID)) for 1))-ascii('A')+1)  as sectorGSM,
(ascii(substring(CellID from (char_length(CellID)-1) for 1))-ascii('A')+1)  as sectorLTE,
regexp_replace(AntennaPattern, '\\', '_') as antfile,
*
from allimport);

select * from TempCells;

SELECT regexp_matches(substring (CGI from 8 for (char_length(CGI)-7)), '(.*)(\-)(.*)'), CGI
from allimport;

SELECT regexp_matches(substring (CGI from 8 for (char_length(CGI)-7)), '(.*)(\-)(.*)'), CGI
from allimport;

drop table tempmeas6id;

create table tempmeas6id(
Position	geometry(Point,4326), 
Times		char(50),		
Longitude	double precision,	Latitude	double precision,Speed		integer,
Operatorname	char(20),		Operator	integer,
CGI		bigint,			Cellname	char(30),	Node	char(20),
CellID		integer,		LAC		integer,
NetworkTech	char(10),		NetworkMode	char(10),
RxLev		integer,		Qual		integer,	
SNR		char(3),		CQI		char(3),
LTERSSI		char(3),		ARFCN		integer,
DL_bitrate	integer,		UL_bitrate	integer,	PSC	integer,
Altitude	real,			Height		real,		Accuracy	real,
Location	char(3),		Status		char(3),
PINGAVG		integer,		PINGMIN		integer,
PINGMAX		integer,		PINGSTDEV	integer,	PINGLOSS	integer,		
TESTDOWNLINK	integer,		TESTUPLINK	integer,
TESTDOWNLINKMAX	integer,		TESTUPLINKMAX	integer,
Test_Status	char(20),		DataConnection_Type	char(3),
DataConnection_Info	smallint,	Layer		smallint,
Filemark	char(20),		IMEI		bigint,
Distance	char(4),		Bearing		real,
IP		char(30),		TA		smallint,
EVENT		char(50),		R1		smallint,
R2		smallint,		R3		smallint,	
R4		smallint,		R5		smallint,	
NTech1		char(5),		NCellName1	char(50),
NCellid1	integer,		NLAC1		integer,
NCell1		integer,		NARFCN1		integer,
NRxLev1		integer,		NQual1		integer,
NDistance1	real,			NBearing1	real,
NTech2		char(5),		NCellName2	char(50),
NCellid2	integer,		NLAC2		integer,
NCell2		integer,		NARFCN2		integer,
NRxLev2		integer,		NQual2		integer,
NDistance2	real,			NBearing2	real,
NTech3		char(5),		NCellName3	char(50),
NCellid3	integer,		NLAC3		integer,
NCell3		integer,		NARFCN3		integer,
NRxLev3		integer,		NQual3		integer,
NDistance3	real,			NBearing3	real,
NTech4		char(5),		NCellName4	char(50),
NCellid4	integer,		NLAC4		integer,
NCell4		integer,		NARFCN4		integer,
NRxLev4		integer,		NQual4		integer,
NDistance4	real,			NBearing4	real,
NTech5		char(5),		NCellName5	char(50),
NCellid5	integer,		NLAC5		integer,
NCell5		integer,		NARFCN5		integer,
NRxLev5		integer,		NQual5		integer,
NDistance5	real,			NBearing5	real,
NTech6		char(5),		NCellName6	char(50),
NCellid6	integer,		NLAC6		integer,
NCell6		integer,		NARFCN6		integer,
NRxLev6		integer,
id		bigserial primary key
);

select count(*) from tempmeas;

truncate table TPtempmeas31;

create table TPtempmeas31(
Position	geometry(Point,4326),
Times		char(50),		
Longitude	double precision,	Latitude	double precision, Speed	integer,
Operatorname	char(20),		Operator	char(20),
CGI		bigint,			Cellname	char(30),	Node	char(20),
CellID		integer,		LAC		integer,
NetworkTech	char(10),		NetworkMode	char(10),
RxLev		integer,		Qual		integer,	
SNR		real,			CQI		real,
LTERSSI		integer,		ARFCN		integer,
DL_bitrate	integer,		UL_bitrate	integer,	PSC	integer,
Altitude	real,			Height		real,		Accuracy	real,
Location	char(3),		Status		char(3),
PINGAVG		integer,		PINGMIN		integer,
PINGMAX		integer,		PINGSTDEV	integer,	PINGLOSS	integer,		
TESTDOWNLINK	integer,		TESTUPLINK	integer,
TESTDOWNLINKMAX	integer,		TESTUPLINKMAX	integer,
Test_Status	char(20),		DataConnection_Type	char(8),
DataConnection_Info	char(20),	Layer		smallint,
Filemark	char(20),		IMEI		bigint,
Distance	char(4),		Bearing		real,
IP		char(30),		TA		smallint,
EVENT		char(50),		EVENTDETAILS	char(100),
IMSI		char(50),		MSISDN		char(50),
SERVINGTIME	integer,		CELLHEX		char(10),
NTech1		char(5),		NCellName1	char(50),
NCellid1	integer,		NLAC1		integer,
NCell1		integer,		NARFCN1		integer,
NRxLev1		integer,		NQual1		integer,
NDistance1	real,			NBearing1	real,
NTech2		char(5),		NCellName2	char(50),
NCellid2	integer,		NLAC2		integer,
NCell2		integer,		NARFCN2		integer,
NRxLev2		integer,		NQual2		integer,
NDistance2	real,			NBearing2	real,
NTech3		char(5),		NCellName3	char(50),
NCellid3	integer,		NLAC3		integer,
NCell3		integer,		NARFCN3		integer,
NRxLev3		integer,		NQual3		integer,
NDistance3	real,			NBearing3	real,
NTech4		char(5),		NCellName4	char(50),
NCellid4	integer,		NLAC4		integer,
NCell4		integer,		NARFCN4		integer,
NRxLev4		integer,		NQual4		integer,
NDistance4	real,			NBearing4	real,
NTech5		char(5),		NCellName5	char(50),
NCellid5	integer,		NLAC5		integer,
NCell5		integer,		NARFCN5		integer,
NRxLev5		integer,		NQual5		integer,
NDistance5	real,			NBearing5	real,
NTech6		char(5),		NCellName6	char(50),
NCellid6	integer,		NLAC6		integer,
NCell6		integer,		NARFCN6		integer,
NRxLev6		integer,		NQual6		integer,
NDistance6	real,			NBearing6	real,
NTech7		char(5),		NCellName7	char(50),
NCellid7	integer,		NLAC7		integer,
NCell7		integer,		NARFCN7		integer,
NRxLev7		integer,		NQual7		integer,
NDistance7	real,			NBearing7	real,
NTech8		char(5),		NCellName8	char(50),
NCellid8	integer,		NLAC8		integer,
NCell8		integer,		NARFCN8		integer,
NRxLev8		integer,		NQual8		integer,
NDistance8	real,			NBearing8	real,
NTech9		char(5),		NCellName9	char(50),
NCellid9	integer,		NLAC9		integer,
NCell9		integer,		NARFCN9		integer,
NRxLev9		integer,		NQual9		integer,
NDistance9	real,			NBearing9	real,
NTech10		char(5),		NCellName10	char(50),
NCellid10	integer,		NLAC10		integer,
NCell10		integer,		NARFCN10	integer,
NRxLev10	integer,		NQual10		integer,
NDistance10	real,			NBearing10	real,
NTech11		char(5),		NCellName11	char(50),
NCellid11	integer,		NLAC11		integer,
NCell11		integer,		NARFCN11	integer,
NRxLev11	integer,		NQual11		integer,
NDistance11	real,			NBearing11	real,
NTech12		char(5),		NCellName12	char(50),
NCellid12	integer,		NLAC12		integer,
NCell12		integer,		NARFCN12	integer,
NRxLev12	integer,		NQual12		integer,
NDistance12	real,			NBearing12	real,
NTech13		char(5),		NCellName13	char(50),
NCellid13	integer,		NLAC13		integer,
NCell13		integer,		NARFCN13	integer,
NRxLev13	integer,		NQual13		integer,
NDistance13	real,			NBearing13	real,
NTech14		char(5),		NCellName14	char(50),
NCellid14	integer,		NLAC14		integer,
NCell14		integer,		NARFCN14	integer,
NRxLev14	integer,		NQual14		integer,
NDistance14	real,			NBearing14	real,
NTech15		char(5),		NCellName15	char(50),
NCellid15	integer,		NLAC15		integer,
NCell15		integer,		NARFCN15	integer,
NRxLev15	integer,		NQual15		integer,
NDistance15	real,			NBearing15	real,
NTech16		char(5),		NCellName16	char(50),
NCellid16	integer,		NLAC16		integer,
NCell16		integer,		NARFCN16	integer,
NRxLev16	integer,		NQual16		integer,
NDistance16	real,			NBearing16	real,
NTech17		char(5),		NCellName17	char(50),
NCellid17	integer,		NLAC17		integer,
NCell17		integer,		NARFCN17	integer,
NRxLev17	integer,		NQual17		integer,
NDistance17	real,			NBearing17	real,
NTech18		char(5),		NCellName18	char(50),
NCellid18	integer,		NLAC18		integer,
NCell18		integer,		NARFCN18	integer,
NRxLev18	integer,		NQual18		integer,
NDistance18	real,			NBearing18	real,
NTech19		char(5),		NCellName19	char(50),
NCellid19	integer,		NLAC19		integer,
NCell19		integer,		NARFCN19	integer,
NRxLev19	integer,		NQual19		integer,
NDistance19	real,			NBearing19	real,
NTech20		char(5),		NCellName20	char(50),
NCellid20	integer,		NLAC20		integer,
NCell20		integer,		NARFCN20	integer,
NRxLev20	integer,		NQual20		integer,
NDistance20	real,			NBearing20	real,
NTech21		char(5),		NCellName21	char(50),
NCellid21	integer,		NLAC21		integer,
NCell21		integer,		NARFCN21	integer,
NRxLev21	integer,		NQual21		integer,
NDistance21	real,			NBearing21	real,
NTech22		char(5),		NCellName22	char(50),
NCellid22	integer,		NLAC22		integer,
NCell22		integer,		NARFCN22	integer,
NRxLev22	integer,		NQual22		integer,
NDistance22	real,			NBearing22	real,
NTech23		char(5),		NCellName23	char(50),
NCellid23	integer,		NLAC23		integer,
NCell23		integer,		NARFCN23	integer,
NRxLev23	integer,		NQual23		integer,
NDistance23	real,			NBearing23	real,
NTech24		char(5),		NCellName24	char(50),
NCellid24	integer,		NLAC24		integer,
NCell24		integer,		NARFCN24	integer,
NRxLev24	integer,		NQual24		integer,
NDistance24	real,			NBearing24	real,
NTech25		char(5),		NCellName25	char(50),
NCellid25	integer,		NLAC25		integer,
NCell25		integer,		NARFCN25	integer,
NRxLev25	integer,		NQual25		integer,
NDistance25	real,			NBearing25	real,
NTech26		char(5),		NCellName26	char(50),
NCellid26	integer,		NLAC26		integer,
NCell26		integer,		NARFCN26	integer,
NRxLev26	integer,		NQual26		integer,
NDistance26	real,			NBearing26	real,
NTech27		char(5),		NCellName27	char(50),
NCellid27	integer,		NLAC27		integer,
NCell27		integer,		NARFCN27	integer,
NRxLev27	integer,		NQual27		integer,
NDistance27	real,			NBearing27	real,
NTech28		char(5),		NCellName28	char(50),
NCellid28	integer,		NLAC28		integer,
NCell28		integer,		NARFCN28	integer,
NRxLev28	integer,		NQual28		integer,
NDistance28	real,			NBearing28	real,
NTech29		char(5),		NCellName29	char(50),
NCellid29	integer,		NLAC29		integer,
NCell29		integer,		NARFCN29	integer,
NRxLev29	integer,		NQual29		integer,
NDistance29	real,			NBearing29	real,
NTech30		char(5),		NCellName30	char(50),
NCellid30	integer,		NLAC30		integer,
NCell30		integer,		NARFCN30	integer,
NRxLev30	integer,		NQual30		integer,
NDistance30	real,			NBearing30	real,
NTech31		char(5),		NCellName31	char(50),
NCellid31	integer,		NLAC31		integer,
NCell31		integer,		NARFCN31	integer,
NRxLev31	integer,
id		bigserial primary key);

create table tempmeas18(
Times		char(50),		
Longitude	double precision,	Latitude	double precision, Speed	integer,
Operatorname	char(20),		Operator	char(20),
CGI		bigint,			Cellname	char(30),	Node	char(20),
CellID		integer,		LAC		integer,
NetworkTech	char(10),		NetworkMode	char(10),
RxLev		integer,		Qual		integer,	
SNR		real,			CQI		real,
LTERSSI		integer,		ARFCN		integer,
DL_bitrate	integer,		UL_bitrate	integer,	PSC	integer,
Altitude	real,			Height		real,		Accuracy	real,
Location	char(3),		Status		char(3),
PINGAVG		integer,		PINGMIN		integer,
PINGMAX		integer,		PINGSTDEV	integer,	PINGLOSS	integer,		
TESTDOWNLINK	integer,		TESTUPLINK	integer,
TESTDOWNLINKMAX	integer,		TESTUPLINKMAX	integer,
Test_Status	char(20),		DataConnection_Type	char(8),
DataConnection_Info	char(20),	Layer		smallint,
Filemark	char(20),		IMEI		bigint,
Distance	char(4),		Bearing		real,
IP		char(30),		TA		smallint,
EVENT		char(50),		EVENTDETAILS	char(100),
IMSI		char(50),		MSISDN		char(50),
SERVINGTIME	integer,		CELLHEX		char(10),
NTech1		char(5),		NCellName1	char(50),
NCellid1	integer,		NLAC1		integer,
NCell1		integer,		NARFCN1		integer,
NRxLev1		integer,		NQual1		integer,
NDistance1	real,			NBearing1	real,
NTech2		char(5),		NCellName2	char(50),
NCellid2	integer,		NLAC2		integer,
NCell2		integer,		NARFCN2		integer,
NRxLev2		integer,		NQual2		integer,
NDistance2	real,			NBearing2	real,
NTech3		char(5),		NCellName3	char(50),
NCellid3	integer,		NLAC3		integer,
NCell3		integer,		NARFCN3		integer,
NRxLev3		integer,		NQual3		integer,
NDistance3	real,			NBearing3	real,
NTech4		char(5),		NCellName4	char(50),
NCellid4	integer,		NLAC4		integer,
NCell4		integer,		NARFCN4		integer,
NRxLev4		integer,		NQual4		integer,
NDistance4	real,			NBearing4	real,
NTech5		char(5),		NCellName5	char(50),
NCellid5	integer,		NLAC5		integer,
NCell5		integer,		NARFCN5		integer,
NRxLev5		integer,		NQual5		integer,
NDistance5	real,			NBearing5	real,
NTech6		char(5),		NCellName6	char(50),
NCellid6	integer,		NLAC6		integer,
NCell6		integer,		NARFCN6		integer,
NRxLev6		integer,		NQual6		integer,
NDistance6	real,			NBearing6	real,
NTech7		char(5),		NCellName7	char(50),
NCellid7	integer,		NLAC7		integer,
NCell7		integer,		NARFCN7		integer,
NRxLev7		integer,		NQual7		integer,
NDistance7	real,			NBearing7	real,
NTech8		char(5),		NCellName8	char(50),
NCellid8	integer,		NLAC8		integer,
NCell8		integer,		NARFCN8		integer,
NRxLev8		integer,		NQual8		integer,
NDistance8	real,			NBearing8	real,
NTech9		char(5),		NCellName9	char(50),
NCellid9	integer,		NLAC9		integer,
NCell9		integer,		NARFCN9		integer,
NRxLev9		integer,		NQual9		integer,
NDistance9	real,			NBearing9	real,
NTech10		char(5),		NCellName10	char(50),
NCellid10	integer,		NLAC10		integer,
NCell10		integer,		NARFCN10	integer,
NRxLev10	integer,		NQual10		integer,
NDistance10	real,			NBearing10	real,
NTech11		char(5),		NCellName11	char(50),
NCellid11	integer,		NLAC11		integer,
NCell11		integer,		NARFCN11	integer,
NRxLev11	integer,		NQual11		integer,
NDistance11	real,			NBearing11	real,
NTech12		char(5),		NCellName12	char(50),
NCellid12	integer,		NLAC12		integer,
NCell12		integer,		NARFCN12	integer,
NRxLev12	integer,		NQual12		integer,
NDistance12	real,			NBearing12	real,
NTech13		char(5),		NCellName13	char(50),
NCellid13	integer,		NLAC13		integer,
NCell13		integer,		NARFCN13	integer,
NRxLev13	integer,		NQual13		integer,
NDistance13	real,			NBearing13	real,
NTech14		char(5),		NCellName14	char(50),
NCellid14	integer,		NLAC14		integer,
NCell14		integer,		NARFCN14	integer,
NRxLev14	integer,		NQual14		integer,
NDistance14	real,			NBearing14	real,
NTech15		char(5),		NCellName15	char(50),
NCellid15	integer,		NLAC15		integer,
NCell15		integer,		NARFCN15	integer,
NRxLev15	integer,		NQual15		integer,
NDistance15	real,			NBearing15	real,
NTech16		char(5),		NCellName16	char(50),
NCellid16	integer,		NLAC16		integer,
NCell16		integer,		NARFCN16	integer,
NRxLev16	integer,		NQual16		integer,
NDistance16	real,			NBearing16	real,
NTech17		char(5),		NCellName17	char(50),
NCellid17	integer,		NLAC17		integer,
NCell17		integer,		NARFCN17	integer,
NRxLev17	integer,		NQual17		integer,
NDistance17	real,			NBearing17	real,
NTech18		char(5),		NCellName18	char(50),
NCellid18	integer,		NLAC18		integer,
NCell18		integer,		NARFCN18	integer,
NRxLev18	integer);
,		NQual18		integer,
NDistance18	real,			NBearing18	real,
NTech19		char(5),		NCellName19	char(50),
NCellid19	integer,		NLAC19		integer,
NCell19		integer,		NARFCN19	integer,
NRxLev19	integer,		NQual19		integer,
NDistance19	real,			NBearing19	real,
NTech20		char(5),		NCellName20	char(50),
NCellid20	integer,		NLAC20		integer,
NCell20		integer,		NARFCN20	integer,
NRxLev20	integer,		NQual20		integer,
NDistance20	real,			NBearing20	real,
NTech21		char(5),		NCellName21	char(50),
NCellid21	integer,		NLAC21		integer,
NCell21		integer,		NARFCN21	integer,
NRxLev21	integer,		NQual21		integer,
NDistance21	real,			NBearing21	real,
NTech22		char(5),		NCellName22	char(50),
NCellid22	integer,		NLAC22		integer,
NCell22		integer,		NARFCN22	integer,
NRxLev22	integer,		NQual22		integer,
NDistance22	real,			NBearing22	real,
NTech23		char(5),		NCellName23	char(50),
NCellid23	integer,		NLAC23		integer,
NCell23		integer,		NARFCN23	integer,
NRxLev23	integer,		NQual23		integer,
NDistance23	real,			NBearing23	real,
NTech24		char(5),		NCellName24	char(50),
NCellid24	integer,		NLAC24		integer,
NCell24		integer,		NARFCN24	integer,
NRxLev24	integer,		NQual24		integer,
NDistance24	real,			NBearing24	real,
NTech25		char(5),		NCellName25	char(50),
NCellid25	integer,		NLAC25		integer,
NCell25		integer,		NARFCN25	integer,
NRxLev25	integer,		NQual25		integer,
NDistance25	real,			NBearing25	real,
NTech26		char(5),		NCellName26	char(50),
NCellid26	integer,		NLAC26		integer,
NCell26		integer,		NARFCN26	integer,
NRxLev26	integer,		NQual26		integer,
NDistance26	real,			NBearing26	real,
NTech27		char(5),		NCellName27	char(50),
NCellid27	integer,		NLAC27		integer,
NCell27		integer,		NARFCN27	integer,
NRxLev27	integer,		NQual27		integer,
NDistance27	real,			NBearing27	real,
NTech28		char(5),		NCellName28	char(50),
NCellid28	integer,		NLAC28		integer,
NCell28		integer,		NARFCN28	integer,
NRxLev28	integer,		NQual28		integer,
NDistance28	real,			NBearing28	real,
NTech29		char(5),		NCellName29	char(50),
NCellid29	integer,		NLAC29		integer,
NCell29		integer,		NARFCN29	integer,
NRxLev29	integer,		NQual29		integer,
NDistance29	real,			NBearing29	real,
NTech30		char(5),		NCellName30	char(50),
NCellid30	integer,		NLAC30		integer,
NCell30		integer,		NARFCN30	integer,
NRxLev30	integer,		NQual30		integer,
NDistance30	real,			NBearing30	real,
NTech31		char(5),		NCellName31	char(50),
NCellid31	integer,		NLAC31		integer,
NCell31		integer,		NARFCN31	integer,
NRxLev31	integer,		NQual31		integer,
NDistance31	real,			NBearing31	real,
NTech32		char(5),		NCellName32	char(50),
NCellid32	integer,		NLAC32		integer,
NCell32		integer,		NARFCN32	integer,
NRxLev32	integer,		NQual32		integer,
NDistance32	real,			NBearing32	real,
NTech33		char(5),		NCellName33	char(50),
NCellid33	integer,		NLAC33		integer,
NCell33		integer,		NARFCN33	integer,
NRxLev33	integer,		NQual33		integer,
NDistance33	real,			NBearing33	real,
NTech34		char(5),		NCellName34	char(50),
NCellid34	integer,		NLAC34		integer,
NCell34		integer,		NARFCN34	integer,
NRxLev34	integer,		NQual34		integer,
NDistance34	real,			NBearing34	real,
NTech35		char(5),		NCellName35	char(50),
NCellid35	integer,		NLAC35		integer,
NCell35		integer,		NARFCN35	integer,
NRxLev35	integer,		NQual35		integer,
NDistance35	real,			NBearing35	real,
NTech36		char(5),		NCellName36	char(50),
NCellid36	integer,		NLAC36		integer,
NCell36		integer,		NARFCN36	integer,
NRxLev36	integer,		NQual36		integer,
NDistance36	real,			NBearing36	real
);

insert into tptempmeas31
select ST_SetSRID(ST_MakePoint(Longitude, Latitude),4326), *
from aug2017tempmeas;

insert into aug2017tempmeas 
select * from tempmeas3;

drop table celllist2017;

create table celllist2017 as
select lac, node, cellid, arfcn, psc as pci, 
substring(times from 1 for 10) as datum, count(*) as num
from tptempmeas31
where networktech='4G'
and cellid<10 and psc is not null
group by lac, node, cellid, arfcn, psc, datum
order by PCI;

create table LTElist2017 as
select node, cellid, pci, sum(num) as num 
from celllist2017 
group by node, cellid, pci
order by pci; 

drop table LTElist2017maxtemp;
create table LTElist2017maxtemp as 
select node, cellid, max(num) as maks
from LTElist2017
group by node, cellid;

delete from LTElist2017
using LTElist2017maxtemp
where LTElist2017.node=LTElist2017maxtemp.node
and LTElist2017.cellid=LTElist2017maxtemp.cellid
and num<maks/3

select * from LTElist2017;

create table unfoundPCI as

insert into unfoundPCI
select lac, node as servnode, cellid as servCI, 
9 as NBrank, ncell9 as unknownPCI, count(*) as num
from tempmeasUse2017
where networktech='4G' 
and lac<20000
and ntech9='4G'
and ncell9 not in 
(select pci from LTElist2017)
group by lac, node, cellid, ncell9
order by num desc;

create table PCItoFind as
select unknownPCI, sum(num) as num
from unfoundPCI
group by unknownPCI

delete from PCItoFind where num<60;

select * from unfoundPCI;

create table PCInotFound as
select servnode, servci, unknownpci as nbPci, sum(num) as num
from unfoundPCI
where unknownPCI in (select unknownpci from PCItoFind)
group by servnode, servci, unknownpci
order by num desc;

create table nodesFound as
select distinct node from 

update tptempmeas31 set networktech='3G' where arfcn>2000;

drop table cells;

delete from tptempmeas31 where networktech='4G' and cellid>9;

create table LTEcelllist as 
select distinct node, cellid, pci, sum(num) as num, max(datum) as datum
from celllist2017
where pci is not null
and cellid<10
group by node, cellid, pci 
order by node, cellid, num desc;

drop table LTEcelllist;
select * from LTEcelllist;


create table cells as 
select networktech, cgi, lac, cast(node as int) as node, cellid, arfcn, psc, count(*) as num
from tptempmeas
select siteid, sector, pci, node, cells.cellid, cells.psc 
from cells left outer join allimport2
on( node=siteid and cells.cellid=sector);
group by  networktech, cgi, lac, node, cellid, arfcn, psc
order by networktech, cgi, lac, node, cellid, num desc;

select distinct siteid, sector, pci, node, cells.cellid, cells.psc, num 
from cells left outer join allimport2
on( node=siteid and cells.cellid=sector)
where pci<>cells.psc
and pci is not null;

select distinct ncell1
from tptempmeas
where ncell1 not in (select psc from cells);


create table tempLTErad(
CellIdentity		char(12),
SiteLnumber 		char(12),
Status			char(20),
rachRootSequence	integer,
LTEConfig		char(12),
AssignedCarrier		char(12),
Latitude		double precision,
Longitude		double precision,
AntennaUniqueID		char(100),
AntennaSlot		smallint,
AntennaHeight		real,
Azimuth			real,
MechTilt		real,
ElecTilt		real,
TotalTilt		real,
PrimaryPredictionModel 	char(100),
PrimaryPredictionRadius real,
FeederType		char(100),
FeederLength		real
);

create table tempUMTSrad(
CellIdentity		char(12),
SiteName		char(50),
Tsector 		char(12),
MTNUMTScellID		char(12),
SiteNum			integer,
AssignedCarrier		integer,
AntennaUniqueID		char(100),
AntennaSlot		smallint,
Latitude		double precision,
Longitude		double precision,
AntennaHeight		real,
Azimuth			real,
MechTilt		real,
ElecTilt		real,
TotalTilt		real,
FeederType		char(30),
FeederLength		real,
PrimaryPredictionModel 	char(100),
PrimaryPredictionRadius real,
PrimaryPredictionRes 	real
);

create table tempGSMrad
(SiteID			char(7),
Tsector 		char(7),
SiteNum			integer,
MTNGSMcellID		char(7),
SiteName		char(50),
CellName		char(50),
Status			char(20),
BTSType			char(20),
AntennaUniqueID1	char(100),
AntennaUniqueID2	char(100),
Gain1			real,
Gain2			real,
AntennaSlot1		smallint,
AntennaSlot2		smallint,
AntennaHeight1		real,
AntennaHeight2		real,
Azimuth1		real,
Azimuth2		real,
MechTilt1		real,
ElecTilt1		real,
TotalTilt1		real,
MechTilt2		real,
ElecTilt2		real,
TotalTilt2		real,
EPSG1			integer,	
Latitude1		double precision,
Longitude1		double precision,
EPSG2			integer,
Latitude2		double precision,
Longitude2		double precision,
FeederType1		char(30),
FeederType2		char(30),
Length1			real,
Length2			real,
PrimaryPredictionModel1 char(100),
PrimaryPredictionRadius1 real,
PrimaryPredictionModel2 char(100),
PrimaryPredictionRadius2 real
);

drop table antennalist;

create table tempLTEantList as 
(select distinct overlay(AntennaUniqueID placing '_' from position('\' in AntennaUniqueID) for 1) 
as antenna from tempLTErad);

create table antennalist as
(select antenna from tempLTEantList
where antenna not in 
(select AntName from tempAntPattern)
order by antenna);

select * from antennapattern;

create table tempantpattern as
(select id, antdevicekey, trim(both '.txt' from patternfile) as AntName, patternfile
from antennapattern);

drop table tempantpattern;

insert into site 
(id, status, location)
select distinct siteid, 'Operational', 
ST_SetSRID(ST_MakePoint(Longitude, Latitude),4326)
from CellsInList
where siteid not in
(select id from site)
and occur > 80;

delete from site where sitename is null;
delete from radioinstallation where siteid not in 
(select id from site);

create table tempLTEsite as 
(select cast(trim(both 'L' from sitelnumber) as int) as siteid, 
overlay(AntennaUniqueID placing '_' from position('\' in AntennaUniqueID) for 1) as antenna,
*
from templterad);

select * from tempLTEsite 
where siteid not in 
(select id from site);

update site set location = ST_SetSRID(ST_MakePoint(Latitude, Longitude),4326)
from tempumtsrad
where id=sitenum;

drop table tempGSMsite;

create table BTSs
(Name	CHAR(20),
sensitivity real,
pwr900 real,
pwr1800 real,
pwr3G real,
pwr4G real)

insert into BTSs
(Name, sensitivity, pwr900, pwr1800)
values ('RBS 6201', -111, 43.0, 43.0);

create table tempGSMsite as
select (ascii(substring(tsector from (char_length(tsector)) for 1))-ascii('A')+1)  as sector, 
tempantpattern1.id as antkey1, tempantpattern2.id as antkey2,
tempGSMrad.*, BTSs.*, BTSs.pwr900+Gain1-length1*3.87/100 as EIRP1, 
BTSs.pwr1800+Gain2-length2*5.751/100 as EIRP2  
from tempgsmrad 
left outer join tempantpattern as tempantpattern1 on tempantpattern1.antname=antennauniqueid1
left outer join tempantpattern as tempantpattern2 on tempantpattern2.antname=antennauniqueid2
left outer join BTSs on btstype=name;

update tempGSMsite set antkey2=186 where antkey2 is null and antennauniqueid2 is not null;

//GSM 900 input
insert into radioinstallation 
(id, siteid, sector, techkey, eirp, diversity, 
txpower, txlosses,
txantennaheight, txantpatternkey, txbearing, txmechtilt,
rxsensitivity, rxlosses,
rxantennaheight, rxantpatternkey, rxbearing, rxmechtilt)
(select 100*sitenum+sector as id, sitenum, sector,1, eirp1, true,
pwr900, length1*3.87/100,
antennaheight1, antkey1, azimuth1, mechtilt1,
sensitivity, length1*3.87/100,
antennaheight1, antkey1, azimuth1, mechtilt1 
from tempgsmsite
where antennauniqueid1 is not null);

//GSM 1800 input
insert into radioinstallation 
(id, siteid, sector, techkey, eirp, diversity, 
txpower, txlosses,
txantennaheight, txantpatternkey, txbearing, txmechtilt,
rxsensitivity, rxlosses,
rxantennaheight, rxantpatternkey, rxbearing, rxmechtilt)
(select 100*sitenum+20+sector as id, sitenum, sector,3, eirp2, true,
pwr1800 as txpower, length2*5.75/100,
antennaheight2, antkey2, azimuth2, mechtilt2,
sensitivity, length2*5.75/100,
antennaheight2, antkey2, azimuth2, mechtilt2 
from tempgsmsite
where antennauniqueid2 is not null);

//LTE input 

insert into radioinstallation 
(id, siteid, sector, techkey, eirp, diversity, 
txpower, txlosses,
txantennaheight, txantpatternkey, txbearing, txmechtilt,
rxsensitivity, rxlosses,
rxantennaheight, rxantpatternkey, rxbearing, rxmechtilt)
(select 100*siteid+40+antennaslot as id, siteid, antennaslot,
5, 43-feederlength*5.75/100+gain, true,
43, feederlength*5.75/100,
antennaheight, tempantpattern.id, azimuth, mechtilt,
-115, feederlength*5.75/100,
antennaheight, tempantpattern.id, azimuth, mechtilt 
from templtesite
left outer join tempantpattern on (antenna=antname)
left outer join antennapattern on (tempantpattern.id=antennapattern.id));


//UMTS input 

insert into radioinstallation 
(id, siteid, sector, techkey, eirp, diversity, 
txpower, txlosses,
txantennaheight, txantpatternkey, txbearing, txmechtilt,
rxsensitivity, rxlosses,
rxantennaheight, rxantpatternkey, rxbearing, rxmechtilt)
(select distinct 100*sitenum+30+antennaslot as id, sitenum, antennaslot,
4 as techkey, 43-feederlength*3.87/100+gain as eirp, true,
43 as txpower, feederlength*3.87/100 as txloss,
antennaheight, tempantpattern.id, azimuth, mechtilt,
-120 as sensitivity, feederlength*3.87/100 as rxloss,
antennaheight, tempantpattern.id, azimuth, mechtilt 
from tempumtsrad
left outer join tempantpattern on (antennauniqueid=antname)
left outer join antennapattern on (tempantpattern.id=antennapattern.id)
where assignedcarrier<9000
order by sitenum, antennaslot);

create table celllistImport(
MSC	varchar(10),
BSC	varchar(10),
SITE	varchar(10),
SiteID	integer,
CELL	varchar(10),
bcchno	smallint,
ncc	smallint,
bcc	smallint,
BSIC	smallint,
lac	integer,
ci	integer,
CGIc	varchar(30),
CI2	integer,
LLAC	bigint,
MNCLL	bigint,
CGI	bigint);

create table tempCellist as
(select (ascii(substring(cell from (char_length(cell)) for 1))-ascii('A')+1)  as sector,
CASE when bcchno>512 and bcchno<900 then 3
	else 1
	end as techkey,
	celllistImport.*
	from celllistImport);
	
create table tempGSMcells as
(select radioinstallation.id, tempCellist.* 
from radioinstallation cross join tempCellist
where radioinstallation.siteid = tempcellist.siteid
and radioinstallation.techkey = tempcellist.techkey
and radioinstallation.sector = tempcellist.sector);
	
insert into cell
(id, risector, beacon)
(select ci, id, bsic, cellid from tempGSMcells
order by ci);

insert into frequencyallocationlist
(ci, carrier, channel)
(select ci, 0, bcchno from tempGSMcells);

insert into parametersettings 
(ci, parameterid, setting)
(select distinct ci, 3, LAC from cellsinlist);

delete from parametersettings where parameterid=3;

drop table tempcellist24nov016; 

create table tempcellist24nov016 as
(select cellid, lac, count(*) as num
from tempmeasuse
group by cellid, lac
order by cellid);

insert into tempcellist24nov016
(select ncellid6, nlac6, count(*) as num
from tempmeasuse
group by ncellid6,nlac6
order by ncellid6);

drop table cellist24nov2016;
drop table sitelist;

create table cellist24nov2016 as
(select cellid, sum(num) as allnum, lac, trunc(cellid/10) as dummysite
from tempcellist24nov016
where cellid is not null
group by cellid, dummysite, lac
order by allnum desc);

create table sitelist as 
select dummysite, sum(allnum) as sitetotal from cellist24nov2016
group by dummysite
order by sitetotal desc;

select * from sitelist;

drop table cellsinfo;
create table cellsinfo as
select distinct cellid, lac, allnum, sitelist.dummysite, sitetotal from
cellist24Nov2016 cross join sitelist
where sitelist.dummysite=cellist24Nov2016.dummysite
order by sitetotal desc, allnum desc; 

truncate table tempmeas6id;

insert into tempmeas6id
(select ST_SetSRID(ST_MakePoint(Longitude, Latitude),4326), tempmeas6.*
from tempmeas6);

create table infolackingon as
select distinct site, cellid,lac,allnum,sitetotal from cellsinfo 
where cellid not in
(select id from cell )
and lac>10000
and allnum>500
order by sitetotal desc;

select * from cellsinfo;

drop table CellsInlist;

create table CellsInList as
(select distinct cast(LACCI[1] as int) as LAC, cast(LACCI[3] as int) as CI, 
allnum as occur, tempantpattern.id as antpatternkey, TempCells.*  
from TempCells cross join cellist24nov2016 
left outer join tempantpattern on TempCells.antfile=tempantpattern.antname
where cast(LACCI[1] as int)=cellist24nov2016.lac
and cast(LACCI[3] as int)=cellist24nov2016.cellid
order by allnum desc, siteid, TempCells.cellid, sectorgsm);

create table AllCellsTemp as 
(select distinct cast(LACCI[1] as int) as LAC, cast(LACCI[3] as int) as CI,  TempCells.*  
from TempCells);

select * from CellsInList where ci not in 
(select id from cell);

select * from cellist24Nov2016 
where cellid not in
(select ci from cellsInList)
and cellid not in 
(select id from cell)
and allnum>80
order by allnum desc;

drop table tempmeasUse2017;

create table tempmeasUse2017 as
(select * from tptempmeas31 where
position @ ST_GeomFromText('POLYGON((28.0218767 -26.04847747,
28.00377964 -26.06021314,
28.01408948 -26.08763293,
28.05653532 -26.08927812,
28.05181911 -26.06898747,
28.03898665 -26.05790988,
28.0218767 -26.04847747))',4326));

INSERT INTO customareafilter (areaname,the_geom) VALUES ('Bryanston', 
ST_GeomFromText('POLYGON((28.0218767 -26.04847747,
28.00377964 -26.06021314,
28.01408948 -26.08763293,
28.05653532 -26.08927812,
28.05181911 -26.06898747,
28.03898665 -26.05790988,
28.0218767 -26.04847747))',4326));

truncate table temprad;

insert into temprad 
(select distinct 100*siteid+20+sectorgsm as id, siteid, sectorgsm as sector,3 as techkey, max(eirp), true,
min(antennaheight) as txantennaheight, min(antpatternkey) as txantpatternkey, 
min(azimuth) as txbearing, min(antmechtilt) as txmechtilt,
-110 as sensitivity, min(antennaheight) as rxantennaheight, 
min(antpatternkey) as rxantpatternkey, min(azimuth) as rxbearing, 
min(antmechtilt) as rxmechtilt
from cellsinlist
where (band='1800'
or band='1800/1800'
or cell_layer='DCS_OL')
and occur>80
group by siteid, sectorgsm);

delete from temprad 
where id in (select id from radioinstallation);

insert into radioinstallation 
(id, siteid, sector, techkey, eirp, diversity, 
txantennaheight, txantpatternkey, txbearing, txmechtilt,
rxsensitivity,rxantennaheight, rxantpatternkey, rxbearing, rxmechtilt)
select * from temprad;

insert into cell
(id, risector)
select distinct ci, 100*siteid+20+sectorgsm
from cellsinlist
where (band='1800'
or band='1800/1800'
or cell_layer='DCS_OL')
and ci not in (select id from cell);

select * from cellsinlist;


select * from antennapattern 
order by patternfile;

update radioinstallation set EIRP=cellsInlist.EIRP
from cellsinlist
where radioinstallation.siteid=cellsinlist.siteid
and radioinstallation.sector=cellsinlist.sectorgsm
and cellsinlist.cell_layer='DCS_OL'
and radioinstallation.techkey=3;

update tempmeasUse set ncellid6=null 
where ncellid6 not in
(select id from cell);

insert into testpoint
(id, timeofmeas, originaltp, height, location)
(select id,
cast(('2016-11-24 '||substr(times,12,2)||':'||substr(times,15,2)||':'||substr(times,18,2)) as timestamp),
id, 1.5 as height, position
from tempmeasuse);

insert into measurement
(ci, tp, measvalue)
select ncellid6, id, nrxlev6
from tempmeasuse
where ncellid6 is not null;

delete from testpoint 
where id not in
(select distinct tp from measurement);

create table verkeer
(Datum	char(10),
CELL	char(8),
NE	char(8),
MAX_PAYLOAD	real,
MAX_ERLANG	real,
Site	integer,
Sector	smallint,
Slot	smallint);

create table traffic as
(select distinct lac, ci, site, sector, cellid, 
		sum(max_payload) as payload, sum(max_erlang) as erlang 
from verkeer cross join cellsinlist
where site=siteid
and sector=sectorgsm
group by lac, ci, site, sector, cellid 
order by site, sector);

update cell set pstraffic=payload
from traffic where traffic.ci=cell.id;

SELECT radioinstallation.id as radid, siteid, ST_AsText(location) as location, 
eirp, max(cell.txpower) as txpower, txlosses, rxlosses, rxsensitivity, 
txantpatternkey, txbearing, txmechtilt, rxantpatternkey, rxbearing, rxmechtilt, 
txantennaheight, rxantennaheight, btlfreq, spacing, bandwidth, downlink, uplink, 
sum(cstraffic) as cstraffic, sum(pstraffic) as pstraffic 
FROM radioinstallation CROSS JOIN cell CROSS JOIN technology CROSS JOIN site 
WHERE techkey=technology.id and siteid=site.id and radioinstallation.id=risector 
and location @ ST_GeomFromText('POLYGON((27.976 -26.036,28.075 -26.036,28.075 -26.109,27.976 -26.109,27.976 -26.036))',4326) 
group by radid, siteid, location, eirp, 
txlosses, rxlosses,txantpatternkey, txbearing, txmechtilt, 
rxantpatternkey, rxbearing, rxmechtilt, txantennaheight, rxantennaheight, 
btlfreq, spacing, bandwidth, downlink, uplink, rxsensitivity;

create table GSMtraffic(
CELL	char(10),
ERLANG	real,
MBYTES	real,
Site	integer,
Sector	smallint);

create table UMTStraffic(
CELL	char(10),
site	integer,
ERLANG	real,
MBYTES	real,
Sector	smallint,
Slot	smallint);


create table LTEtraffic(
CELL	char(10),
SITE	integer,
MBYTES	real,
Sector	smallint,
Slot	smallint);


select imei, count(*) as num 
from tempmeasuse
where accuracy <=20
group by imei;

delete from testpoint where positionsource>1;
where id not in
(select id from tempmeasuse where accuracy<=20);

truncate table ;


truncate table test;

insert into test 
select tp, servci, ta from 
testpointauxGSM 
where tp in
(select id from tempmeasuse
where imei = 354187074241040
and accuracy<=20
and tp%5=0);

select count(*) from test;

drop table LTEta;

create table LTEta as 
select tptempmeas.id as tp, 
round(CAST(ST_Distance_Spheroid(site.location, position, 'SPHEROID["WGS 84",6378137,298.257223563]') As numeric),2) As distance, ta
from site cross join tptempmeas
where site.id=cast(node as int);

drop table LTEsimta;

create table LTESimta as 
select LTEta.*, round(distance/156.142-0.5) as simTA, ta-round(distance/156.142-0.5) as delta
from LTEta
order by distance desc;

select delta, count(*)
from LTEsimta
group by delta
order by delta;

create table ServNodesFound as
select distinct node from 
tempmeasuse2017
where lac<10000
and networktech='4G';

truncate table pcimissing;

insert into pcimissing 
select min(substring(times from 1 for 10)) as eerste, 
max(substring(times from 1 for 10)) as laaste, count(*) as num,
ncell8 
from tempmeasuse2017
where ntech8='4G' and networktech='4G' and networkmode='LTE'
and node::integer in (select siteid from sitelist)
and lac < 10000 and cellid<10 and ncell8 not in 
(select pci from celllist2017
where node::integer in 
(select siteid from sitelist))
group by ncell8;  

select min(eerste) as eerste, max(laaste) as laaste,  ncell1 as pci, sum(num) as num
from pcimissing
where ncell1 in (select nbpci from pcioutofregion)
group by ncell1

delete from tempmeasuse2017
where networktech = '4G'
and networkmode = 'LTE'
and node::integer not in
(select siteid from sitelist);

select min(substring(times from 1 for 10)) as eerste, 
max(substring(times from 1 for 10)) as laaste, 
arfcn, cellid, psc, count(*) as num
from tempmeasuse2017
where  networktech='3G' 
and lac > 10000 and cellid>2
and arfcn is not null 
and psc is not null
group by cellid, arfcn, psc
order by cellid, arfcn, psc;  

select count(*) 
from tempmeasuse2017
where  networktech='3G';  