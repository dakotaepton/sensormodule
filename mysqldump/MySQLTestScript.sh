#!/bin/bash

TEMPFILE=/tmp/$$.tmp
user="root"
db="GasSensorDB"
file="testOutput.txt"
export MYSQL_PWD=toor

# Clear TempFile
echo 0 > $TEMPFILE

# Displays output to stdout and log file
echolog()
(
    echo "$@"
    echo "$@" >> ${file}
)

# Checks return value
check()
(
    if [ $? -eq 0 ]; then
        echolog "Sucess"
        # Store count in a temp file so we can access
        #    outside of subshell
        successCount=$[$(cat $TEMPFILE) +1]
        echo $successCount > $TEMPFILE
    fi
)

# If an output file already exists; zero it
#     otherwise create file
if [ -f ${file} ]; then
    truncate -s 0 ${file}
else
    echo -n > ${file}
fi

# Test if a specific relational query can be performed
echolog "---Relational Query for Manufacturer & Sensor---"
mysql -u ${user} -D ${db} -e "SELECT Sensor.PartNumber, Manufacturer.Name FROM Sensor INNER JOIN Manufacturer ON Manufacturer.ManufacturerID=1" >> ${file}

check
echolog "-------------------------------------"

# Test if a general SELECT FROM query can be done
echolog "---Gas SELECT all---" 
mysql -u ${user} -D ${db} -e "SELECT * FROM Gas;" >> ${file}

check
echolog "-------------------------------------"

echolog "---Manufacturer SELECT all---" 
mysql -u ${user} -D ${db} -e "SELECT * FROM Manufacturer;" >> ${file}

check
echolog "-------------------------------------"


echolog "---Sensor SELECT all---"
mysql -u ${user} -D ${db} -e "SELECT * FROM Sensor;" >> ${file}

check
echolog "-------------------------------------"


echolog "---Detects SELECT all---"
mysql -u ${user} -D ${db} -e "SELECT * FROM Detects;" >> ${file}

check
echolog "-------------------------------------"

successCount=$(cat ${TEMPFILE})
echolog "${successCount} / 5 Tests Succeeded"
echo -e "\nPlease see testOutput.txt for query results"

# Delete TEMPFILE
unlink $TEMPFILE
