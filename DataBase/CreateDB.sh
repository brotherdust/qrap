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
# find the following *.sql files on your machine and change PostGISDir accordingly.
# Hint: Look under the "installed files" in Synaptic properties
# for postgres-$.$-postgis to see where the files are stored
PostGISDir=/usr/share/postgresql/9.5/contrib/postgis-2.2/
createlang plpgsql -d qrap
echo "running postgis.sql"
psql qrap -f ${PostGISDir}postgis.sql
echo "running spatial_ref_sys.sql"
psql qrap -f ${PostGISDir}spatial_ref_sys.sql
# echo "running postgis_comments.sql"
# psql qrap -f ${PostGISDir}postgis_comments.sql
# echo "running rtpostgis.sql"
# psql qrap -f ${PostGISDir}rtpostgis.sql
# echo "running raster_comments.sql"
# psql qrap -f ${PostGISDir}raster_comments.sql
# echo "running topology.sql"
# psql qrap -f ${PostGISDir}topology/topology.sql
# echo "running topology_comments.sql"
# psql qrap -f ${PostGISDir}doc/topology_comments.sql
psql -c "ALTER DATABASE qrap OWNER TO postgres"

make
./qraputil c
exit

