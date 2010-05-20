#
#	Note: 	All usernames are:postgres
#		All passwords are:postqrap
#		To change the postgres password,edit the following in terminal:
#			
echo "changing passwd"
psql -c "alter user postgres password 'postqrap'"

# Getting rid of the old and creating the new
echo "refreshing database"
psql -c "DROP DATABASE qrap"
psql -c "DROP ROLE engine"
psql -c "CREATE DATABASE qrap"
pause
# PostGIS stuff
echo "activiting postgis"
# find the following *.sql files on your machine and change PostGISDir accordingly 
PostGISDir=/usr/share/postgresql/8.4/contrib/postgis-1.5/
createlang plpgsql -d qrap
pause
psql qrap -f ${PostGISDir}postgis.sql
psql qrap -f ${PostGISDir}spatial_ref_sys.sql
PostGISDir=/usr/share/postgresql/8.4/contrib/
psql qrap -f ${PostGISDir}postgis_comments.sql
psql -c "ALTER DATABASE qrap OWNER TO postgres"

make
./qraputil c
exit

