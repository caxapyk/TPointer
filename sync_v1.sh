#!/bin/sh

# 21 * * * /root/sync_tpointer.sh

corpus=115

sql='
    TRUNCATE topography.main;
    INSERT INTO topography.main
        SELECT
        tpointer.tpointer.`id`,
        tpointer.tpointer.`floor`,
        tpointer.storage.`name`,
        tpointer.tpointer.`compartment`,
        tpointer.tpointer.`shelving`,
        tpointer.tpointer.`cupboard`,
        tpointer.tpointer.`shelf`,
        tpointer.fund.`number`,
        tpointer.tpointer.`inventory`,
        tpointer.tpointer.`records`,
        tpointer.tpointer.`note`,
        (CASE
            WHEN tpointer.feature.`name` = "Неучтено" THEN 1
            WHEN tpointer.feature.`name` = "Неописано" THEN 2
            ELSE NULL
        END) AS feature
        FROM tpointer.tpointer
        LEFT JOIN tpointer.storage ON tpointer.tpointer.`storage`=tpointer.storage.`id`
        LEFT JOIN tpointer.fund ON tpointer.tpointer.`fund`=tpointer.fund.`id`
        LEFT JOIN tpointer.feature ON tpointer.tpointer.`feature`=tpointer.feature.`id`
        LEFT JOIN tpointer.corpus ON tpointer.storage.`corpus`=tpointer.corpus.`id`
        WHERE tpointer.corpus.`id`='"$corpus"';'

mysql --host=services.lsk.gaorel.ru --user=root --password=antilopagnu --execute="$sql"
