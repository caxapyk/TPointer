#include "syncdialog.h"
#include "ui_syncdialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

SyncDialog::SyncDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncDialog)
{
    ui->setupUi(this);

    m_corpusModel = new CorpusModel;
    m_corpusModel->select();
    ui->cB_corpus->setModel(m_corpusModel);
    ui->cB_corpus->setModelColumn(1);

    connect(ui->pB_sync, &QPushButton::clicked, this, &SyncDialog::sync);
}

SyncDialog::~SyncDialog()
{
    delete ui;
}

void SyncDialog::sync()
{
    if (!ui->gB_v1x->isChecked()) {
        QMessageBox::critical(this, tr("Synchronization..."), tr("Choose version"));
        return;
    }

    QSqlQuery query(QString("TRUNCATE %1; "
                            "INSERT INTO %1 "
                                "SELECT "
                                "tpointer.tpointer.`id`, "
                                "tpointer.tpointer.`floor`, "
                                "tpointer.storage.`name`, "
                                "tpointer.tpointer.`compartment`, "
                                "tpointer.tpointer.`shelving`, "
                                "tpointer.tpointer.`cupboard`, "
                                "tpointer.tpointer.`shelf`, "
                                "tpointer.fund.`number`, "
                                "tpointer.tpointer.`inventory`, "
                                "tpointer.tpointer.`records`, "
                                "tpointer.tpointer.`note`, "
                                "(CASE "
                                    "WHEN tpointer.feature.`name` = \"Неучтено\" THEN 1 "
                                    "WHEN tpointer.feature.`name` = \"Неописано\" THEN 2 "
                                    "ELSE NULL "
                                "END) AS feature "
                                "FROM tpointer.tpointer "
                                "LEFT JOIN tpointer.storage ON tpointer.tpointer.`storage`=tpointer.storage.`id` "
                                "LEFT JOIN tpointer.fund ON tpointer.tpointer.`fund`=tpointer.fund.`id` "
                                "LEFT JOIN tpointer.feature ON tpointer.tpointer.`feature`=tpointer.feature.`id` "
                                "LEFT JOIN tpointer.corpus ON tpointer.storage.`corpus`=tpointer.corpus.`id` "
                                "WHERE tpointer.corpus.`id`=%2;")
                    .arg(ui->lE_database->text())
                    .arg(m_corpusModel->index(ui->cB_corpus->currentIndex(), 0).data().toString()));

    query.exec();

    if(!query.lastError().isValid()) {
        ui->pTE_result->setPlainText(tr("Synchronization is successfull!"));
    } else {
        ui->pTE_result->setPlainText(query.lastError().text());
    }
}
