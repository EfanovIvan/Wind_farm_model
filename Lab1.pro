#-------------------------------------------------
#
# Project created by QtCreator 2020-02-13T21:24:10
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
win32:RC_ICONS = $$PWD/resources/Icon/wind_engine.ico
CONFIG += c++11
CONFIG += qaxcontainer

SOURCES += \
        windows/CellWindow.cpp \
        windows/CellWindowWithoutContactor.cpp \
        windows/ChargingStationWindow.cpp \
        utils/CheckSelection.cpp \
        ClickableLabel.cpp \
        utils/CoefficientValidator.cpp \
        utils/CustomValidator.cpp \
        models/DieselModel.cpp \
        tabs/DieselTab.cpp \
        utils/DigitalValidator.cpp \
        tabs/DistributionTab.cpp \
        windows/DoubleCellWindow.cpp \
        utils/DoubleValidator.cpp \
        models/FuseModel.cpp \
        windows/FuseWindow.cpp \
        models/IndustrialModel.cpp \
        tabs/IndustrialTab.cpp \
        windows/InfoWindow.cpp \
        models/LineProxyModel.cpp \
        tabs/LineTab.cpp \
        models/LinesModel.cpp \
        tabs/LivestockTab.cpp \
        utils/LoadDataFromFile.cpp \
        windows/LoginWindow.cpp \
        windows/MainWindow.cpp \
        models/ProxyCoefficientPower.cpp \
        models/ProxyIndustrialModel.cpp \
        models/ProxyLineModel.cpp \
        models/ProxySelectGeneratorModel.cpp \
        tabs/ResidentialTab.cpp \
        models/ResultModel.cpp \
        tabs/ResultTab.cpp \
        utils/SharedUtils.cpp \
        models/SpeedPowerModel.cpp \
        windows/SpeedPowerWindow.cpp \
        models/TableCoefficientsPower.cpp \
        utils/ValidaTableLineEdit.cpp \
        models/WGModel.cpp \
        models/WGProxyModel.cpp \
        tabs/WindGeneratorTab.cpp \
        main.cpp \
        qcustomplot.cpp \
        xlsx/doc/snippets/doc_src_qtxlsx.cpp \
        xlsx/xlsxabstractooxmlfile.cpp \
        xlsx/xlsxabstractsheet.cpp \
        xlsx/xlsxcell.cpp \
        xlsx/xlsxcellformula.cpp \
        xlsx/xlsxcellrange.cpp \
        xlsx/xlsxcellreference.cpp \
        xlsx/xlsxchart.cpp \
        xlsx/xlsxchartsheet.cpp \
        xlsx/xlsxcolor.cpp \
        xlsx/xlsxconditionalformatting.cpp \
        xlsx/xlsxcontenttypes.cpp \
        xlsx/xlsxdatavalidation.cpp \
        xlsx/xlsxdocpropsapp.cpp \
        xlsx/xlsxdocpropscore.cpp \
        xlsx/xlsxdocument.cpp \
        xlsx/xlsxdrawing.cpp \
        xlsx/xlsxdrawinganchor.cpp \
        xlsx/xlsxformat.cpp \
        xlsx/xlsxmediafile.cpp \
        xlsx/xlsxnumformatparser.cpp \
        xlsx/xlsxrelationships.cpp \
        xlsx/xlsxrichstring.cpp \
        xlsx/xlsxsharedstrings.cpp \
        xlsx/xlsxsimpleooxmlfile.cpp \
        xlsx/xlsxstyles.cpp \
        xlsx/xlsxtheme.cpp \
        xlsx/xlsxutility.cpp \
        xlsx/xlsxworkbook.cpp \
        xlsx/xlsxworksheet.cpp \
        xlsx/xlsxzipreader.cpp \
        xlsx/xlsxzipwriter.cpp

HEADERS += \
        windows/CellWindow.h \
        windows/CellWindowWithoutContactor.h \
        windows/ChargingStationWindow.h \
        utils/CheckSelection.h \
        utils/CoefficientValidator.h \
        utils/CustomValidator.h \
        models/DieselModel.h \
        tabs/DieselTab.h \
        utils/DigitalValidator.h \
        tabs/DistributionTab.h \
        windows/DoubleCellWindow.h \
        utils/DoubleValidator.h \
        models/FuseModel.h \
        windows/FuseWindow.h \
        models/IndustrialModel.h \
        tabs/IndustrialTab.h \
        windows/InfoWindow.h \
        models/LineProxyModel.h \
        tabs/LineTab.h \
        models/LinesModel.h \
        tabs/LivestockTab.h \
        utils/LoadDataFromFile.h \
        windows/LoginWindow.h \
        windows/MainWindow.h \
        models/ProxyIndustrialModel.h \
        models/ProxyLineModel.h \
        models/ProxySelectGeneratorModel.h \
        models/Proxycoefficientpower.h \
        tabs/ResidentialTab.h \
        models/ResultModel.h \
        tabs/ResultTab.h \
        utils/SharedUtils.h \
        models/SpeedPowerModel.h \
        windows/SpeedPowerWindow.h \
        tabs/TabIndex.h \
        models/TableCoefficientsPower.h \
        utils/ValidaTableLineEdit.h \
        models/WGModel.h \
        models/WGProxyModel.h \
        tabs/WindGeneratorTab.h \
        ClickableLabel.h \
        models/proxylinemodel.h \
        proxylinemodel.h \
        qcustomplot.h \
        models/wgmodel.h \
        xlsx/xlsxabstractooxmlfile.h \
        xlsx/xlsxabstractooxmlfile_p.h \
        xlsx/xlsxabstractsheet.h \
        xlsx/xlsxabstractsheet_p.h \
        xlsx/xlsxcell.h \
        xlsx/xlsxcell_p.h \
        xlsx/xlsxcellformula.h \
        xlsx/xlsxcellformula_p.h \
        xlsx/xlsxcellrange.h \
        xlsx/xlsxcellreference.h \
        xlsx/xlsxchart.h \
        xlsx/xlsxchart_p.h \
        xlsx/xlsxchartsheet.h \
        xlsx/xlsxchartsheet_p.h \
        xlsx/xlsxcolor_p.h \
        xlsx/xlsxconditionalformatting.h \
        xlsx/xlsxconditionalformatting_p.h \
        xlsx/xlsxcontenttypes_p.h \
        xlsx/xlsxdatavalidation.h \
        xlsx/xlsxdatavalidation_p.h \
        xlsx/xlsxdocpropsapp_p.h \
        xlsx/xlsxdocpropscore_p.h \
        xlsx/xlsxdocument.h \
        xlsx/xlsxdocument_p.h \
        xlsx/xlsxdrawing_p.h \
        xlsx/xlsxdrawinganchor_p.h \
        xlsx/xlsxformat.h \
        xlsx/xlsxformat_p.h \
        xlsx/xlsxglobal.h \
        xlsx/xlsxmediafile_p.h \
        xlsx/xlsxnumformatparser_p.h \
        xlsx/xlsxrelationships_p.h \
        xlsx/xlsxrichstring.h \
        xlsx/xlsxrichstring_p.h \
        xlsx/xlsxsharedstrings_p.h \
        xlsx/xlsxsimpleooxmlfile_p.h \
        xlsx/xlsxstyles_p.h \
        xlsx/xlsxtheme_p.h \
        xlsx/xlsxutility_p.h \
        xlsx/xlsxworkbook.h \
        xlsx/xlsxworkbook_p.h \
        xlsx/xlsxworksheet.h \
        xlsx/xlsxworksheet_p.h \
        xlsx/xlsxzipreader_p.h \
        xlsx/xlsxzipwriter_p.h

FORMS += \
        forms/authorization.ui \
        forms/cell.ui \
        forms/chargingstation.ui \
        forms/diesel.ui \
        forms/disconnectorcell.ui \
        forms/distribution.ui \
        forms/doublecell.ui \
        forms/fuse.ui \
        forms/industrial.ui \
        forms/infiwindow.ui \
        forms/linetab.ui \
        forms/livestock.ui \
        forms/mainwindow.ui \
        forms/residential.ui \
        forms/result.ui \
        forms/speedpower.ui \
        forms/windgenerator.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    rs.qrc

DISTFILES += \
    resources/Icon/Downloads_Folder.png \
    resources/Icon/exelIcon.png \
    xlsx/doc/qtxlsx.qdocconf \
    xlsx/doc/src/examples.qdoc \
    xlsx/doc/src/qtxlsx-index.qdoc \
    xlsx/doc/src/qtxlsx.qdoc \
    xlsx/doc/src/usage.qdoc \
    xlsx/qtxlsx.pri

SUBDIRS += \
    xlsx/doc/snippets/doc_src_qtxlsx.pro \
    xlsx/xlsx.pro

include(xlsx/qtxlsx.pri)
