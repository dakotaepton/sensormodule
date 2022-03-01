SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE IF EXISTS Gas;
DROP TABLE IF EXISTS Detects;
DROP TABLE IF EXISTS Brand;
DROP TABLE IF EXISTS Sensor;
SET FOREIGN_KEY_CHECKS = 1;


CREATE TABLE Gas (
  GasID     int         AUTO_INCREMENT,
  Fullname  varchar(32) NOT NULL,
  Name      varchar(16) NOT NULL,
  PRIMARY KEY (GasID)
);


CREATE TABLE Detects (
  SensorID                     int         NOT NULL,
  GasID                        int         NOT NULL,
  MeasurementUnits             varchar(8)  DEFAULT NULL,
  MinReading                   double      DEFAULT NULL,
  MaxReading                   double      DEFAULT NULL,
  Resolution                   double      DEFAULT NULL,
  OvergasLimit                 int         DEFAULT NULL,
  MinSensitivity               double      DEFAULT NULL,
  MaxSensitivity               double      DEFAULT NULL,
  SensitivityRatio             varchar(10) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  SensitivityTestConcentration double      DEFAULT NULL,
  TTestType                    varchar(4)  DEFAULT NULL,
  TTestResponse                int         DEFAULT NULL,
  ResponseStartLevel           int         DEFAULT NULL,
  ResponseEndLevel             int         DEFAULT NULL,
  ZeroCurrent                  varchar(24) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (SensorID, GasID)
);


CREATE TABLE Brand (
    BrandID int         NOT NULL AUTO_INCREMENT,
    Name    varchar(32) NOT NULL,
    PRIMARY KEY (BrandID)
);


CREATE TABLE Sensor (
  SensorID            int         NOT NULL AUTO_INCREMENT,
  PartNumber          varchar(32) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  Alias               varchar(32) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  Technology          varchar(32) DEFAULT NULL,
  ActivePart          tinyint(1)  DEFAULT NULL,
  LastReview          date        DEFAULT NULL,
  DatasheetReview     double      DEFAULT NULL,
  Series              varchar(10) DEFAULT NULL,
  Diameter            double      DEFAULT NULL,
  Height              double      DEFAULT NULL,
  PinCount            int         DEFAULT NULL,
  PinLength           double      DEFAULT NULL,
  MaxWeight           double      DEFAULT NULL,
  MinTemperature      double      DEFAULT NULL,
  MaxTemperature      double      DEFAULT NULL,
  MinPressure         double      DEFAULT NULL,
  MaxPressure         double      DEFAULT NULL,
  MinHumidity         double      DEFAULT NULL,
  MaxHumidity         double      DEFAULT NULL,
  Bias                int         DEFAULT NULL,
  MinLoad             double      DEFAULT NULL,
  MaxLoad             double      DEFAULT NULL,
  MaxSignalDrift      varchar(16) DEFAULT NULL,
  SignalDriftInterval int         DEFAULT NULL,
  ExpectedLife        int         DEFAULT NULL,
  Warranty            int         DEFAULT NULL,
  GasCount            int         DEFAULT NULL,
  BrandID             int         NOT NULL,
  PRIMARY KEY (SensorID),
  FOREIGN KEY (BrandID) REFERENCES Brand(BrandID) ON DELETE CASCADE ON UPDATE CASCADE
);

INSERT INTO Brand VALUES (1, 'Alphasense');
INSERT INTO Brand VALUES (2, 'CityTech');
INSERT INTO Brand VALUES (3, 'Membrapor');
INSERT INTO Brand VALUES (4, 'Sensoric');
INSERT INTO Brand VALUES (5, 'Winsen');

INSERT INTO Gas VALUES ( 1, 'Ammonia'           , 'NH3');
INSERT INTO Gas VALUES ( 2, 'Arsine'            , 'AsH3');
INSERT INTO Gas VALUES ( 3, 'Carbon Monoxide'   , 'CO');
INSERT INTO Gas VALUES ( 4, 'Chlorine'          , 'Cl2');
INSERT INTO Gas VALUES ( 5, 'Chlorine Dioxide'  , 'ClO2');
INSERT INTO Gas VALUES ( 6, 'Diborane'          , 'B2H6');
INSERT INTO Gas VALUES ( 7, 'Ethylene'          , 'C2H4');
INSERT INTO Gas VALUES ( 8, 'Ethylene Oxide'    , 'EtO');
INSERT INTO Gas VALUES ( 9, 'Ethylene Oxide'    , 'C2H4O');
INSERT INTO Gas VALUES (10, 'Formaldehyde'      , 'CH2O');
INSERT INTO Gas VALUES (11, 'Nitrogen Dioxide'  , 'NO2');
INSERT INTO Gas VALUES (12, 'Hydrogen'          , 'H2');
INSERT INTO Gas VALUES (13, 'Hydrogen Bromide'  , 'HBr');
INSERT INTO Gas VALUES (14, 'Hydrogen Chloride' , 'HCl');
INSERT INTO Gas VALUES (15, 'Hydrogen Cyanide'  , 'HCN');
INSERT INTO Gas VALUES (16, 'Hydrogen Fluoride' , 'HF');
INSERT INTO Gas VALUES (17, 'Hydrogen Peroxide' , 'H2O2');
INSERT INTO Gas VALUES (18, 'Hydrogen Selenide' , 'H2Se');
INSERT INTO Gas VALUES (19, 'Hydrogen Sulfide'  , 'H2S');
INSERT INTO Gas VALUES (20, 'Nitric Oxide'      , 'NO');
INSERT INTO Gas VALUES (21, 'Nitrogen Dioxide'  , 'NO2');
INSERT INTO Gas VALUES (22, 'Oxygen'            , 'O2');
INSERT INTO Gas VALUES (23, 'Ozone'             , 'O3');
INSERT INTO Gas VALUES (24, 'Phosphine'         , 'PH3');
INSERT INTO Gas VALUES (25, 'Silane'            , 'SiH4');
INSERT INTO Gas VALUES (26, 'Sulfur Dioxide'    , 'SO2');
INSERT INTO Gas VALUES (27, 'Tetrahydrothiopene', 'THT');
INSERT INTO Gas VALUES (28, 'Tetrahydrothiopene', 'C4H8S');
INSERT INTO Gas VALUES (29, 'Carboxylic Acid'   , 'RCOOH');
INSERT INTO Gas VALUES (30, 'Tertiary Alcohol'  , 'R3COH');
INSERT INTO Gas VALUES (31, 'Hydrazine'         , 'N2H4');
INSERT INTO Gas VALUES (32, 'Phosgene'          , 'COCl2');
INSERT INTO Gas VALUES (33, 'Fluoride'          , 'F2');
INSERT INTO Gas VALUES (34, 'Mercaptan'         , 'CH4S');

INSERT INTO Sensor VALUES (1  , 'CL2-A1'        , 'CL2-A1'        , 'EC', 1, NULL, 2017.03, '4-series', 20.2, 16.5, 3, 4.3, 6   , -20, 50, 80     , 120    , 15, 90, NULL, 33   , 33   , '10' , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (2  , 'COH-A2'        , 'COH-A2'        , 'EC', 1, NULL, 2017.03, '4-series', 20.2, 16.5, 4, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '4'  , 12  , 24  , NULL, 2, 1);
INSERT INTO Sensor VALUES (3  , 'CO-A4'         , 'CO-A4'         , 'EC', 1, NULL, 2014.10, '4-series', 20.2, 16.5, 4, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 33   , 100  , '10' , 12  , 36  , NULL, 1, 1);
INSERT INTO Sensor VALUES (4  , 'CO-AE'         , 'CO-AE'         , 'EC', 1, NULL, 2017.03, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '1'  , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (5  , 'CO-AF'         , 'CO-AF'         , 'EC', 1, NULL, 2017.09, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '8'  , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (6  , 'CO-AX'         , 'CO-AX'         , 'EC', 1, NULL, 2014.07, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '6'  , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (7  , 'ETO-A1'        , 'ETO-A1'        , 'EC', 1, NULL, 2016.06, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, 300 , 10   , 47   , NULL , NULL, 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (8  , 'H2-AF'         , 'H2-AF'         , 'EC', 1, NULL, 2016.06, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , NULL , NULL, 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (9  , 'H2S-A4'        , 'H2S-A4'        , 'EC', 1, NULL, 2016.06, '4-series', 20.2, 16.5, 4, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 33   , 100  , '20' , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (10 , 'H2S-AE'        , 'H2S-AE'        , 'EC', 1, NULL, 2017.03, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , NULL , NULL, 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (11 , 'H2S-A1'        , 'H2S-A1'        , 'EC', 1, NULL, 2015.12, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '3'  , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (12 , 'H2S-AH'        , 'H2S-AH'        , 'EC', 1, NULL, 2015.12, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '2'  , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (13 , 'HCL-A1'        , 'HCL-A1'        , 'EC', 1, NULL, 2018.02, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 33   , NULL , NULL, NULL, NULL, 1, 1);
INSERT INTO Sensor VALUES (14 , 'HCN-A1'        , 'HCN-A1'        , 'EC', 1, NULL, 2016.06, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 33   , NULL , NULL, 12  , NULL, 1, 1);
INSERT INTO Sensor VALUES (15 , 'LFO2-A4'       , 'LFO2-A4'       , 'EC', 1, NULL, 2019.02, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 5 , 95, -600, NULL , NULL , '1'  , 3   , 48  , NULL, 1, 1);
INSERT INTO Sensor VALUES (16 , 'NO-A4'         , 'NO-A4'         , 'EC', 1, NULL, 2016.02, '4-series', 20.2, 16.5, 4, 4.3, 6   , -30, 50, 80     , 120    , 15, 85, 200 , 33   , 100  , '-20', 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (17 , 'NO2-A1'        , 'NO2-A1'        , 'EC', 1, NULL, 2015.09, '4-series', 20.2, 16.5, 3, 4.3, 6   , -20, 50, 80     , 120    , 15, 90, NULL, 33   , 33   , '-40', 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (18 , 'NO2-AE'        , 'NO2-AE'        , 'EC', 1, NULL, 2017.03, '4-series', 20.2, 16.5, 3, 4.3, 6   , -20, 50, 80     , 120    , 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (19 , 'NO2-A43F'      , 'NO2-A43F'      , 'EC', 1, NULL, 2018.11, '4-series', 20.2, 16.5, 4, 4.3, 6   , -30, 40, 80     , 120    , 15, 85, NULL, 33   , 100  , '-40', 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (20 , 'NO-A1'         , 'NO-A1'         , 'EC', 1, NULL, 2017.09, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, 300 , 10   , 47   , '5'  , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (21 , 'NO-AE'         , 'NO-AE'         , 'EC', 1, NULL, 2012.01, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, 300 , 10   , 47   , NULL , NULL, 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (22 , 'O2-A1'         , 'O2-A1'         , 'EC', 1, NULL, 2017.07, '4-series', 20.3, 17.4, 3, 4.3, 16  , -30, 55, 80     , 120    , 5 , 95, NULL, 47   , 100  , '1'  , 3   , 12  , NULL, 1, 1);
INSERT INTO Sensor VALUES (23 , 'O2-A2'         , 'O2-A2'         , 'EC', 1, NULL, 2017.01, '4-series', 20.3, 17.4, 3, 4.3, 16  , -30, 55, 80     , 120    , 5 , 95, NULL, 47   , 100  , '1'  , 3   , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (24 , 'OX-A431'       , 'OX-A431'       , 'EC', 1, NULL, 2018.11, '4-series', 20.2, 16.5, 4, 4.3, 6   , -30, 40, 80     , 120    , 15, 85, NULL, 33   , 100  , '-40', 12  , 24  , NULL, 2, 1);
INSERT INTO Sensor VALUES (25 , 'PH3-A1'        , 'PH3-A1'        , 'EC', 1, NULL, 2012.12, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 20, 90, NULL, 10   , 33   , '10' , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (26 , 'SO2-AE'        , 'SO2-AE'        , 'EC', 1, NULL, 2017.03, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '4'  , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (27 , 'SO2-A4'        , 'SO2-A4'        , 'EC', 1, NULL, 2018.09, '4-series', 20.2, 16.5, 4, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 33   , 100  , '15' , 12  , 36  , NULL, 1, 1);
INSERT INTO Sensor VALUES (28 , 'SO2-AF'        , 'SO2-AF'        , 'EC', 1, NULL, 2015.09, '4-series', 20.2, 16.5, 3, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '4'  , 12  , 24  , NULL, 1, 1);
INSERT INTO Sensor VALUES (29 , 'SOH-A2'        , 'SOH-A2'        , 'EC', 1, NULL, 2017.03, '4-series', 20.2, 16.5, 4, 4.3, 6   , -30, 50, 80     , 120    , 15, 90, NULL, 10   , 47   , '6'  , 12  , 24  , NULL, 2, 1);
INSERT INTO Sensor VALUES (30 , 'AB231-801'     , '2CF3'          , 'EC', 1, NULL, 2017.02, '4-series', 20.4, 16.6, 3, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '10' , 12  , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (31 , '2112B2005'     , '4CF+'          , 'EC', 1, NULL, 2016.10, '4-series', 20.2, 16.6, 3, 4  , 5   , -20, 40, 91.1925, 111.458, 15, 90, NULL, 5    , 5    , '5'  , 12  , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (32 , 'AH200-800'     , '4CL'           , 'EC', 1, NULL, 2017.09, '4-series', 20.4, 16.6, 3, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (33 , '2112B2055R'    , '4CM'           , 'EC', 1, NULL, 2016.05, '4-series', 20.2, 16.6, 3, 4  , 5   , -40, 55, 80     , 120    , 15, 95, NULL, 5    , 5    , '5'  , 12  , 24  , 24  , 1, 2);
INSERT INTO Sensor VALUES (34 , 'ABC05-800'     , '4COSH'         , 'EC', 1, NULL, 2018.03, '4-series', 20  , 16.6, 4, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '30' , 36  , 36  , NULL, 2, 2);
INSERT INTO Sensor VALUES (35 , 'AT204-800'     , '4ETO'          , 'EC', 1, NULL, 2017.09, '4-series', 20.4, 16.6, 3, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '5'  , 12  , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (36 , 'AC200-800'     , '4H'            , 'EC', 1, NULL, 2017.03, '4-series', 20.4, 16.6, 3, 4.3, 5   , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (37 , 'AC206-800'     , '4HLM'          , 'EC', 1, NULL, 2017.03, '4-series', 20.4, 16.6, 3, 4.3, 5   , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (38 , 'AJ200-800'     , '4HN'           , 'EC', 1, NULL, 2017.09, '4-series', 20.4, 16.6, 3, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , NULL , NULL, 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (39 , '2112B2025'     , '4HS+'          , 'EC', 1, NULL, 2016.10, '4-series', 20.2, 16.6, 3, 4  , 5   , -20, 40, 91.1925, 111.458, 15, 90, NULL, 5    , 5    , '20' , 12  , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (40 , 'AE204-803'     , '4HYT'          , 'EC', 1, NULL, 2017.09, '4-series', 20.4, 16.6, 3, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (41 , 'AG200-800'     , '4ND'           , 'EC', 1, NULL, 2017.09, '4-series', 20.4, 16.6, 3, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (42 , 'AF4NT-800'     , '4NT'           , 'EC', 1, NULL, 2017.09, '4-series', 20.4, 16.6, 3, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (43 , 'AA783-33H'     , '4OxLL'         , 'EC', 1, NULL, 2016.05, '4-series', 20.4, 16.6, 3, 4  , 5.2 , -40, 60, 81.06  , 121.59 , 15, 90, -600, NULL , NULL , '5'  , 60  , 60  , NULL, 1, 2);
INSERT INTO Sensor VALUES (44 , 'AP210-800'     , '4PH-Fast'      , 'EC', 1, NULL, 2017.09, '4-series', 20.4, 16.6, 3, 4.3, 5   , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 2);
INSERT INTO Sensor VALUES (45 , '2112B2015R'    , '4S Rev. 2'     , 'EC', 1, NULL, 2016.05, '4-series', 20.2, 16.6, 3, 4.1, 4.5 , -20, 50, 81.06  , 121.59 , 15, 90, NULL, 10   , 10   , '10' , 12  , 24  , 12  , 1, 2);
INSERT INTO Sensor VALUES (46 , 'Acid/M-100'    , 'Acid/M-100'    , 'EC', 1, NULL, 2018.08, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (47 , 'C2H4/M-1500'   , 'C2H4/M-1500'   , 'EC', 1, NULL, 2010.07, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '5'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (48 , 'C2H4/M-200'    , 'C2H4/M-200'    , 'EC', 1, NULL, 2010.06, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '5'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (49 , 'CH2O/M-10-2E'  , 'CH2O/M-10-2E'  , 'EC', 1, NULL, 2008.11, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (50 , 'CH2O/M-10'     , 'CH2O/M-10'     , 'EC', 1, NULL, 2016.10, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 33   , '2'  , 1   , 36  , 12  , 1, 3);
INSERT INTO Sensor VALUES (52 , 'CH2O/M-1000'   , 'CH2O/M-1000'   , 'EC', 1, NULL, 2016.10, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (53 , 'CH2O/M-50'     , 'CH2O/M-50'     , 'EC', 1, NULL, 2016.12, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (54 , 'Cl2/M-20'      , 'Cl2/M-20'      , 'EC', 1, NULL, 2018.06, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 33   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (55 , 'Cl2/M-200'     , 'Cl2/M-200'     , 'EC', 1, NULL, 2013.12, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (56 , 'Cl2/M-2EG'     , 'Cl2/M-2EG'     , 'EC', 1, NULL, 2015.10, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (57 , 'ClO2/M-5'      , 'ClO2/M-5'      , 'EC', 1, NULL, 2018.05, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 33   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (58 , 'CO/MF-1000'    , 'CO/MF-1000'    , 'EC', 1, NULL, 2008.02, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (59 , 'CO/MF-200'     , 'CO/MF-200'     , 'EC', 1, NULL, 2016.08, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (60 , 'CO/MF-2E'      , 'CO/MF-2E'      , 'EC', 1, NULL, 2005.03, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (61 , 'CO/MF-500'     , 'CO/MF-500'     , 'EC', 1, NULL, 2016.08, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (62 , 'CO/MFA-500'    , 'CO/MFA-500'    , 'EC', 1, NULL, 2007.02, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (63 , 'ETO/M-10'      , 'ETO/M-10'      , 'EC', 1, NULL, 2018.10, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (64 , 'ETO/M-100'     , 'ETO/M-100'     , 'EC', 1, NULL, 2018.10, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (65 , 'ETO/M-1000'    , 'ETO/M-1000'    , 'EC', 1, NULL, 2018.10, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (66 , 'ETO/M-500'     , 'ETO/M-500'     , 'EC', 1, NULL, 2018.10, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (67 , 'H2/M-1000'     , 'H2/M-1000'     , 'EC', 1, NULL, 2017.06, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (68 , 'H2/M-4000'     , 'H2/M-4000'     , 'EC', 1, NULL, 2017.06, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (69 , 'H2/M-40000'    , 'H2/M-40000'    , 'EC', 1, NULL, 2016.03, '4-series', 20  , 16.4, 3, 4.3, 5.4 , 0  , 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (70 , 'H2O2/MB-100'   , 'H2O2/MB-100'   , 'EC', 1, NULL, 2018.01, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (71 , 'H2O2/MB-500'   , 'H2O2/MB-500'   , 'EC', 1, NULL, 2018.01, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (72 , 'H2S/M-100'     , 'H2S/M-100'     , 'EC', 1, NULL, 2008.02, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (73 , 'H2S/M-200-2E'  , 'H2S/M-200-2E'  , 'EC', 1, NULL, 2009.06, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (74 , 'H2S/M-2000'    , 'H2S/M-2000'    , 'EC', 1, NULL, 2009.10, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (75 , 'H2S/M-50'      , 'H2S/M-50'      , 'EC', 1, NULL, 2008.02, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (76 , 'H2S/M-500'     , 'H2S/M-500'     , 'EC', 1, NULL, 2013.07, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (77 , 'HCl/M-20'      , 'HCl/M-20'      , 'EC', 1, NULL, 2017.02, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 20   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (78 , 'HCl/M-200'     , 'HCl/M-200'     , 'EC', 1, NULL, 2017.02, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 20   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (79 , 'HCN/M-50'      , 'HCN/M-50'      , 'EC', 1, NULL, 2009.08, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (80 , 'NH3/MR-100-2E' , 'NH3/MR-100-2E' , 'EC', 1, NULL, 2009.04, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -10, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (81 , 'NH3/MR-100'    , 'NH3/MR-100'    , 'EC', 1, NULL, 2007.03, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -10, 40, 101.325, 101.325, 15, 90, NULL, 10   , 10   , '5'  , 6   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (82 , 'NH3/MR-1000-2E', 'NH3/MR-1000-2E', 'EC', 1, NULL, 2014.04, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -10, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 20   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (83 , 'NH3/MR-1000'   , 'NH3/MR-1000'   , 'EC', 1, NULL, 2007.12, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -10, 50, 101.325, 101.325, 15, 90, NULL, 10   , 20   , '5'  , 6   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (84 , 'NH3/MR-10000'  , 'NH3/MR-10000'  , 'EC', 1, NULL, 2015.06, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -10, 50, 101.325, 101.325, 15, 90, NULL, 10   , 20   , '5'  , 6   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (85 , 'NH3/MR-2000'   , 'NH3/MR-2000'   , 'EC', 1, NULL, 2013.10, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -10, 50, 101.325, 101.325, 15, 90, NULL, 10   , 20   , '5'  , 6   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (86 , 'NH3/MR-500'    , 'NH3/MR-500'    , 'EC', 1, NULL, 2010.05, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -10, 50, 101.325, 101.325, 15, 90, NULL, 10   , 20   , '5'  , 6   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (87 , 'NH3/MR-5000'   , 'NH3/MR-5000'   , 'EC', 1, NULL, 2014.04, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -10, 50, 101.325, 101.325, 15, 90, NULL, 10   , 20   , '5'  , 6   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (88 , 'NO/M-1000'     , 'NO/M-1000'     , 'EC', 1, NULL, 2007.01, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (89 , 'NO/M-25'       , 'NO/M-25'       , 'EC', 1, NULL, 2013.11, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (90 , 'NO/M-250'      , 'NO/M-250'      , 'EC', 1, NULL, 2006.12, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (91 , 'NO2/M-100'     , 'NO2/M-100'     , 'EC', 1, NULL, 2008.07, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (92 , 'NO2/M-20'      , 'NO2/M-20'      , 'EC', 1, NULL, 2007.12, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (93 , 'NO2/M-2E'      , 'NO2/M-2E'      , 'EC', 1, NULL, 2015.10, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (94 , 'NO2/M-2EG'     , 'NO2/M-2EG'     , 'EC', 1, NULL, 2017.05, '4-series', 20.5, 16.4, 2, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (96 , 'NO2/M-500'     , 'NO2/M-500'     , 'EC', 1, NULL, 2016.01, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 33   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (97 , 'O2/M-1'        , 'O2/M-1'        , 'EC', 1, NULL, 2014.08, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -40, 50, 81.06  , 121.59 , 15, 90, -600, 10   , 10   , '4'  , 36  , 36  , 12  , 1, 3);
INSERT INTO Sensor VALUES (98 , 'O2/M-100'      , 'O2/M-100'      , 'EC', 1, NULL, 2018.08, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, -600, 10   , 33   , '4'  , 36  , 36  , 12  , 1, 3);
INSERT INTO Sensor VALUES (100, 'O2/MP-100'     , 'O2/MP-100'     , 'EC', 1, NULL, 2018.08, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 81.06  , 121.59 , 15, 90, -600, 10   , 10   , '4'  , 36  , 36  , 12  , 1, 3);
INSERT INTO Sensor VALUES (101, 'O2/MT-100'     , 'O2/MT-100'     , 'EC', 1, NULL, 2018.10, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 50, 95, -600, 10   , 10   , '4'  , 36  , 36  , 12  , 1, 3);
INSERT INTO Sensor VALUES (102, 'O3/M-100'      , 'O3/M-100'      , 'EC', 1, NULL, 2018.10, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 33   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (104, 'O3/M-5'        , 'O3/M-5'        , 'EC', 1, NULL, 2007.05, '4-series', 20  , 16.4, 3, 4.3, 5.4 , -20, 45, 91.1925, 111.458, 15, 90, NULL, 10   , 33   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (105, 'PH3/M-20'      , 'PH3/M-20'      , 'EC', 1, NULL, 2007.03, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (106, 'PH3/M-200'     , 'PH3/M-200'     , 'EC', 1, NULL, 2015.11, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (107, 'PH3/M-2000'    , 'PH3/M-2000'    , 'EC', 1, NULL, 2008.12, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (108, 'PH3/M-5'       , 'PH3/M-5'       , 'EC', 1, NULL, 2007.04, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (109, 'SiH4/M-50'     , 'SiH4/M-50'     , 'EC', 1, NULL, 2007.03, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (110, 'SO2/M-100-2E'  , 'SO2/M-100-2E'  , 'EC', 1, NULL, 2007.10, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (111, 'SO2/M-20'      , 'SO2/M-20'      , 'EC', 1, NULL, 2006.06, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (112, 'SO2/M-2E'      , 'SO2/M-2E'      , 'EC', 1, NULL, 2010.04, '4-series', 20  , 16.4, 2, 4.3, 5.4 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (113, 'SO2/MF-100'    , 'SO2/MF-100'    , 'EC', 1, NULL, 2014.12, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (114, 'SO2/MF-1000'   , 'SO2/MF-1000'   , 'EC', 1, NULL, 2018.04, '4-series', 20.5, 16.4, 3, 4.3, 5.5 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , NULL , NULL, 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (115, 'SO2/MF-10000'  , 'SO2/MF-10000'  , 'EC', 1, NULL, 2008.09, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (116, 'SO2/MF-20'     , 'SO2/MF-20'     , 'EC', 1, NULL, 2014.06, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -40, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (117, 'SO2/MF-200'    , 'SO2/MF-200'    , 'EC', 1, NULL, 2015.08, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (118, 'SO2/MF-2000'   , 'SO2/MF-2000'   , 'EC', 1, NULL, 2014.01, '4-series', 20  , 16.4, 3, 4.3, 5.6 , -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , 12  , 1, 3);
INSERT INTO Sensor VALUES (119, '0731-021-30049', 'AsH3 2E 1'     , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 2, 4.3, NULL, -20, 40, NULL   , NULL   , 20, 90, NULL, NULL , NULL , '5'  , 6   , 18  , 10  , 1, 4);
INSERT INTO Sensor VALUES (120, '0731-237-30049', 'AsH3 3E 1 F LT', 'EC', 1, NULL, 2011.06, '4-series', 20.4, 16.6, 3, 4.3, 4.5 , -20, 40, 91.1925, 111.458, 10, 95, NULL, 1500 , 1500 , '5'  , 6   , 24  , 10  , 1, 4);
INSERT INTO Sensor VALUES (121, '0731-337-30049', 'AsH3 3E 1 LT'  , 'EC', 1, NULL, 2011.06, '4-series', 20.4, 16.6, 3, 4.3, 4.5 , -20, 40, 91.1925, 111.458, 10, 95, NULL, 1500 , 1500 , '5'  , 6   , 24  , 10  , 1, 4);
INSERT INTO Sensor VALUES (122, '0831-337-30049', 'B2H6 3E 1 LT'  , 'EC', 1, NULL, 2011.06, '4-series', 20.4, 16.6, 3, 4.3, 4.5 , -20, 40, 91.1925, 111.458, 10, 95, NULL, 1500 , 1500 , '5'  , 6   , 24  , 10  , 1, 4);
INSERT INTO Sensor VALUES (123, '0436-032-30049', 'Cl2 3E 10'     , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (124, '0441-032-30049', 'Cl2 3E 50'     , 'EC', 1, NULL, 2012.04, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (125, '2731-331-30049', 'ClO2 3E 1 O'   , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (126, '0248-034-30049', 'CO 3E 300'     , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -40, 50, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 48  , 24  , 1, 4);
INSERT INTO Sensor VALUES (127, '1731-031-14049', 'COCl2 3E 1'    , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '5'  , 6   , 12  , 7   , 1, 4);
INSERT INTO Sensor VALUES (128, '1431-031-30049', 'F2 3E 1'       , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -10, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '5'  , 1   , 18  , 12  , 1, 4);
INSERT INTO Sensor VALUES (129, '0361-034-30049', 'H2 3E 1%'      , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 24  , 18  , 1, 4);
INSERT INTO Sensor VALUES (130, '0364-034-30049', 'H2 3E 4%'      , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 95, NULL, NULL , NULL , '10' , 6   , 24  , 18  , 1, 4);
INSERT INTO Sensor VALUES (131, '0141-124-30049', 'H2S 2E 50 S'   , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 2, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 48  , 24  , 1, 4);
INSERT INTO Sensor VALUES (132, '0145-034-30049', 'H2S 3E 100'    , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -40, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 48  , 24  , 1, 4);
INSERT INTO Sensor VALUES (133, '0145-134-30049', 'H2S 3E 100 S'  , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -40, 50, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 48  , 24  , 1, 4);
INSERT INTO Sensor VALUES (134, '1139-034-30049', 'HCl HBr 3E 30' , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 95, 200 , NULL , NULL , '3'  , 1   , 24  , 12  , 2, 4);
INSERT INTO Sensor VALUES (135, '1639-221-30049', 'HCN 2E 30 F'   , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 2, 4.3, NULL, -40, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '5'  , 1   , 18  , 12  , 1, 4);
INSERT INTO Sensor VALUES (136, '1639-231-30049', 'HCN 3E 30 F'   , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -40, 40, NULL   , NULL   , 15, 95, NULL, NULL , NULL , '5'  , 1   , 18  , 12  , 1, 4);
INSERT INTO Sensor VALUES (137, '1336-932-30049', 'HF 3E 10 SE'   , 'EC', 1, NULL, 2014.09, '4-series', 20.4, 16.6, 3, 4.3, 4.6 , -20, 40, 91.1925, 111.458, 15, 90, NULL, NULL , NULL , '10' , 6   , 18  , 12  , 1, 4);
INSERT INTO Sensor VALUES (138, '2131-021-30049', 'N2H4 2E 1'     , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 2, 4.3, NULL, -10, 40, NULL   , NULL   , 20, 95, NULL, NULL , NULL , '10' , 6   , 12  , 10  , 1, 4);
INSERT INTO Sensor VALUES (139, '1845-031-30049', 'NH3 3E 100'    , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -40, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 18  , 12  , 1, 4);
INSERT INTO Sensor VALUES (140, '1854-031-30049', 'NH3 3E 1000'   , 'EC', 1, NULL, 2017.11, '4-series', 20.4, 16.6, 3, 4.3, 5   , -40, 40, 91.1925, 111.458, 15, 90, NULL, NULL , NULL , '10' , 6   , 18  , NULL, 1, 4);
INSERT INTO Sensor VALUES (141, '1854-932-30049', 'NH3 3E 1000 SE', 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (142, '1845-932-30049', 'NH3 3E 100 SE' , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '5'  , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (143, '1858-932-30049', 'NH3 3E 5000 SE', 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (144, '1850-932-30049', 'NH3 3E 500 SE' , 'EC', 1, NULL, 2008.07, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, 100  , 100  , '5'  , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (145, '2241-032-30049', 'NO2 3E 50'     , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '5'  , 1   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (146, '1945-034-30049', 'NO 3E 100'     , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -15, 40, NULL   , NULL   , 20, 90, 200 , NULL , NULL , '5'  , 1   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (147, '1531-031-30049', 'O3 3E 1'       , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '10' , 6   , 18  , 12  , 1, 4);
INSERT INTO Sensor VALUES (148, '1531-231-30049', 'O3 3E 1 F'     , 'EC', 1, NULL, 2011.11, '4-series', 20.4, 16.6, 3, 4.3, NULL, -20, 40, NULL   , NULL   , 15, 90, NULL, NULL , NULL , '5'  , 1   , 18  , 12  , 1, 4);
INSERT INTO Sensor VALUES (149, '0635-237-30049', 'PH3 3E 5 F LT' , 'EC', 1, NULL, 2011.06, '4-series', 20.4, 16.6, 3, 4.3, 4.5 , -20, 50, 91.1925, 111.458, 10, 95, NULL, 1500 , 1500 , '5'  , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (150, '0635-337-30049', 'PH3 3E 5 LT'   , 'EC', 1, NULL, 2011.06, '4-series', 20.4, 16.6, 3, 4.3, 4.5 , -20, 50, 91.1925, 111.458, 10, 95, NULL, 1500 , 1500 , '5'  , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (151, '3035-337-30049', 'SeH2 3E 5 LT'  , 'EC', 1, NULL, 2011.03, '4-series', 20.4, 16.6, 3, 4.3, 4.5 , -20, 40, 91.1925, 111.458, 10, 95, NULL, 1500 , 1500 , '5'  , 6   , 24  , 10  , 1, 4);
INSERT INTO Sensor VALUES (152, '0941-337-30049', 'SiH4 3E 50 LT' , 'EC', 1, NULL, 2011.06, '4-series', 20.4, 16.6, 3, 4.3, 4.5 , -20, 40, 91.1925, 111.458, 10, 95, NULL, 1500 , 1500 , '5'  , 6   , 24  , 12  , 1, 4);
INSERT INTO Sensor VALUES (153, '2441-021-04049', 'TBM 2E 50'     , 'EC', 1, NULL, 2011.11, '4-series', 20  , 16.4, 2, 4.3, NULL, -10, 40, NULL   , NULL   , 10, 95, NULL, NULL , NULL , '10' , 10  , 12  , 9   , 1, 4);
INSERT INTO Sensor VALUES (154, 'ME2-CO'        , 'ME2-CO'        , 'EC', 1, NULL, NULL   , '4-series', 20  , 16.4, 2, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 200  , 200  , '10' , 12  , 60  , NULL, 1, 5);
INSERT INTO Sensor VALUES (155, 'ME2-O2-Ф20'    , 'ME2-O2-Ф20'    , 'EC', 1, NULL, NULL   , '4-series', 20  , 16.4, 2, 4.3, NULL, -20, 50, 91.1925, 111.458, 0 , 99, NULL, 10000, 10000, '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (156, 'ME3-CH2O'      , 'ME3-CH2O'      , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, 300 , 300  , 300  , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (157, 'ME3-CL2'       , 'ME3-CL2'       , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (158, 'ME3-CO'        , 'ME3-CO'        , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '5'  , 1   , 36  , NULL, 1, 5);
INSERT INTO Sensor VALUES (159, 'ME3-ETO'       , 'ME3-ETO'       , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, 300 , 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (160, 'ME3-H2'        , 'ME3-H2'        , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 90     , 110    , 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (161, 'ME3-H2S'       , 'ME3-H2S'       , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (162, 'ME3-HCL'       , 'ME3-HCL'       , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (163, 'ME3-HF'        , 'ME3-HF'        , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 90     , 110    , 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (164, 'ME3-NH3'       , 'ME3-NH3'       , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '10' , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (165, 'ME3-NO2'       , 'ME3-NO2'       , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (166, 'ME3-O3'        , 'ME3-O3'        , 'EC', 1, NULL, 1.2    , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);
INSERT INTO Sensor VALUES (167, 'ME3-SO2'       , 'ME3-SO2'       , 'EC', 1, NULL, NULL   , '4-series', 20  , 16.4, 3, 4.3, NULL, -20, 50, 91.1925, 111.458, 15, 90, NULL, 10   , 10   , '2'  , 1   , 24  , NULL, 1, 5);

INSERT INTO Detects VALUES (1  , 4 , 'ppm' , 0   , 20   , 0.02 , 50    , -350 , -750 , 'nA/ppm', 10  , 't90', 60  , 0   , 10  , '± 0.4');
INSERT INTO Detects VALUES (2  , 3 , 'ppm' , 0   , 1000 , 0.5  , 5000  , 50   , 100  , 'nA/ppm', 400 , 't90', 35  , 0   , 400 , '-3 to +3.5');
INSERT INTO Detects VALUES (2  , 19, 'ppm' , 0   , 100  , 0.1  , 200   , 450  , 1000 , 'nA/ppm', 20  , 't90', 30  , 0   , 20  , '± 0.25');
INSERT INTO Detects VALUES (3  , 3 , 'ppm' , 0   , 500  , NULL , 2000  , 220  , 375  , 'nA/ppm', 2   , 't90', 20  , 0   , 10  , '-100 to +10');
INSERT INTO Detects VALUES (4  , 3 , 'ppm' , 0   , 10000, 5    , 100000, 10   , 25   , 'nA/ppm', 2000, 't90', 50  , 0   , 2000, '< ± 20');
INSERT INTO Detects VALUES (5  , 3 , 'ppm' , 0   , 5000 , 0.5  , 10000 , 55   , 90   , 'nA/ppm', 400 , 't90', 25  , 0   , 400 , '-5 to +4');
INSERT INTO Detects VALUES (6  , 3 , 'ppm' , 0   , 2000 , 0.5  , 4000  , 55   , 100  , 'nA/ppm', 400 , 't90', 30  , 0   , 400 , '< ± 3');
INSERT INTO Detects VALUES (7  , 8 , 'ppm' , 0   , 100  , 0.1  , 200   , 2000 , 3200 , 'nA/ppm', 20  , 't90', 150 , 0   , 20  , '± 0.6');
INSERT INTO Detects VALUES (8  , 12, 'ppm' , 0   , 2000 , 0.7  , 5000  , 10   , 25   , 'nA/ppm', 400 , 't90', 45  , 0   , 400 , '± 15');
INSERT INTO Detects VALUES (9  , 19, 'ppm' , 0   , 50   , NULL , 100   , 1400 , 1850 , 'nA/ppm', 2   , 't90', 45  , 0   , 2   , '-250 to 100');
INSERT INTO Detects VALUES (10 , 19, 'ppm' , 0   , 2000 , 0.5  , 10000 , 65   , 110  , 'nA/ppm', 400 , 't90', 25  , 0   , 400 , '< ±3');
INSERT INTO Detects VALUES (11 , 19, 'ppm' , 0   , 100  , 0.05 , 500   , 550  , 875  , 'nA/ppm', 20  , 't90', 35  , 0   , 20  , '< ± 0.4');
INSERT INTO Detects VALUES (12 , 19, 'ppm' , 0   , 50   , 0.03 , 250   , 950  , 1450 , 'nA/ppm', 20  , 't90', 30  , 0   , 20  , '< ± 0.2');
INSERT INTO Detects VALUES (13 , 14, 'ppm' , 0   , 100  , 1    , 200   , 80   , 130  , 'nA/ppm', 25  , 't90', 300 , 0   , 25  , '< ±2.5');
INSERT INTO Detects VALUES (14 , 15, 'ppm' , 0   , 100  , 0.05 , 150   , 55   , 85   , 'nA/ppm', 30  , 't90', 70  , 0   , 30  , '< ±2');
INSERT INTO Detects VALUES (15 , 22, '%Vol', 0   , 25   , NULL , NULL  , 80   , 130  , 'μA'    , 20.9, 't90', 17  , 20.9, 0   , NULL);
INSERT INTO Detects VALUES (16 , 20, 'ppm' , 0   , 20   , NULL , 50    , 350  , 550  , 'nA/ppm', 2   , 't90', 25  , 0   , 2   , '10 to 80');
INSERT INTO Detects VALUES (17 , 21, 'ppm' , 0   , 20   , 0.02 , 100   , -250 , -650 , 'nA/ppm', 10  , 't90', 50  , 0   , 10  , '< ± 0.4');
INSERT INTO Detects VALUES (18 , 21, 'ppm' , 0   , 200  , 0.1  , 1000  , -70  , -170 , 'nA/ppm', 10  , 't90', 40  , 0   , 10  , '< ± 1.5');
INSERT INTO Detects VALUES (19 , 21, 'ppm' , 0   , 20   , NULL , 50    , -175 , -450 , 'nA/ppm', 2   , 't90', 60  , 0   , 2   , '-60 to +70');
INSERT INTO Detects VALUES (20 , 20, 'ppm' , 0   , 250  , 0.2  , 800   , 320  , 480  , 'nA/ppm', 50  , 't90', 45  , 0   , 50  , '0 to +2');
INSERT INTO Detects VALUES (21 , 20, 'ppm' , 0   , 5000 , 1    , 10000 , 40   , 80   , 'nA/ppm', 250 , 't90', 75  , 0   , 250 , '0 to 15');
INSERT INTO Detects VALUES (22 , 22, '%Vol', 0   , 25   , NULL , NULL  , 190  , 240  , 'μA'    , 20.9, 't90', 15  , 20.9, 0   , '< 2.5');
INSERT INTO Detects VALUES (23 , 22, '%Vol', 0   , 25   , NULL , NULL  , 80   , 120  , 'μA'    , 20.9, 't90', 15  , 20.9, 0   , '< 2.5');
INSERT INTO Detects VALUES (24 , 23, 'ppm' , 0   , 20   , NULL , 50    , -200 , -650 , 'nA/ppm', 1   , 't90', 45  , 0   , 1   , '-70 to 70');
INSERT INTO Detects VALUES (24 , 21, 'ppm' , 0   , 20   , NULL , 50    , -200 , -550 , 'nA/ppm', 2   , 't90', 45  , 0   , 1   , '-70 to 70');
INSERT INTO Detects VALUES (25 , 24, 'ppm' , 0   , 10   , 0.1  , 75    , 550  , 900  , 'nA/ppm', 11  , 't90', 25  , 0   , 5   , '< ±0.5');
INSERT INTO Detects VALUES (26 , 26, 'ppm' , 0   , 2000 , 1.5  , 10000 , 50   , 80   , 'nA/ppm', 400 , 't90', 33  , 0   , 400 , '< ±5');
INSERT INTO Detects VALUES (27 , 26, 'ppm' , 0   , 50   , NULL , 100   , 320  , 500  , 'nA/ppm', 2   , 't90', 20  , 0   , 2   , '-80 to 80');
INSERT INTO Detects VALUES (28 , 26, 'ppm' , 0   , 50   , 0.1  , 75    , 300  , 550  , 'nA/ppm', 10  , 't90', 35  , 0   , 10  , '< ± 0.6');
INSERT INTO Detects VALUES (29 , 26, 'ppm' , 0   , 20   , 0.2  , 50    , 140  , 250  , 'nA/ppm', 10  , 't90', 15  , 0   , 10  , '< ± 0.2');
INSERT INTO Detects VALUES (29 , 19, 'ppm' , 0   , 100  , 0.1  , 200   , 450  , 900  , 'nA/ppm', 20  , 't90', 25  , 0   , 20  , '± 0.25');
INSERT INTO Detects VALUES (30 , 3 , 'ppm' , 0   , 500  , 1    , 1000  , 30   , 70   , 'nA/ppm', NULL, 't90', 17  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (31 , 3 , 'ppm' , 0   , 500  , NULL , 2000  , 0.055, 0.085, 'μA/ppm', NULL, 't90', 20  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (32 , 4 , 'ppm' , 0   , 10   , 0.1  , 100   , 0.45 , 0.75 , 'μA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (33 , 3 , 'ppm' , 0   , 2000 , 1    , NULL  , 55   , 85   , 'nA/ppm', NULL, 't90', 10  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (34 , 3 , 'ppm' , 0   , 500  , 1    , 1500  , 50   , 110  , 'nA/ppm', NULL, 't90', 35  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (34 , 19, 'ppm' , 0   , 200  , 0.5  , 500   , 500  , 1050 , 'nA/ppm', NULL, 't90', 35  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (35 , 9 , 'ppm' , 0   , 20   , 0.1  , 100   , 1.4  , 2.4  , 'μA/ppm', NULL, 't90', 120 , NULL, NULL, NULL);
INSERT INTO Detects VALUES (36 , 19, 'ppm' , 0   , 100  , 0.1  , 500   , 0.95 , 1.45 , 'μA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (37 , 19, 'ppm' , 0   , 100  , 0.1  , 500   , 0.95 , 1.45 , 'μA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (38 , 15, 'ppm' , 0   , 50   , 0.5  , NULL  , 0.08 , 0.12 , 'μA/ppm', NULL, 't90', 200 , NULL, NULL, NULL);
INSERT INTO Detects VALUES (39 , 19, 'ppm' , 0   , 100  , NULL , 500   , 0.55 , 0.85 , 'μA/ppm', NULL, 't90', 20  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (40 , 12, 'ppm' , 0   , 1000 , 2    , 2000  , 5    , 25   , 'nA/ppm', NULL, 't90', 90  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (41 , 21, 'ppm' , 0   , 20   , 0.1  , 150   , 0.45 , 0.75 , 'μA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (42 , 20, 'ppm' , 0   , 250  , 0.5  , 1000  , 0.32 , 0.48 , 'μA/ppm', NULL, 't90', 40  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (43 , 22, '%Vol', 0   , 25   , NULL , 30    , 80   , 130  , 'μA/ppm', NULL, 't90', 15  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (44 , 24, 'ppm' , 0   , 5    , 0.05 , 20    , 1.4  , 2    , 'μA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (45 , 26, 'ppm' , 0   , 20   , 0.1  , 150   , 0.4  , 0.6  , 'μA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (46 , 29, 'ppm' , 0   , 100  , 1    , 200   , -150 , -30  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (47 , 7 , 'ppm' , 0   , 1500 , 5    , 2000  , 8    , 16   , 'nA/ppm', NULL, 't80', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (48 , 7 , 'ppm' , 0   , 200  , 1    , 500   , 40   , 90   , 'nA/ppm', NULL, 't80', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (49 , 10, 'ppm' , 0   , 10   , 0.02 , 30    , 1400 , 3000 , 'nA/ppm', NULL, 't90', NULL, NULL, NULL, NULL);
INSERT INTO Detects VALUES (50 , 10, 'ppm' , 0   , 10   , 0.01 , 30    , 3400 , 5800 , 'nA/ppm', NULL, 't60', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (52 , 10, 'ppm' , 0   , 1000 , 1    , 2000  , 45   , 75   , 'nA/ppm', NULL, 't60', 40  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (53 , 10, 'ppm' , 0   , 50   , 0.5  , 100   , 700  , 1300 , 'nA/ppm', NULL, 't60', 40  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (54 , 4 , 'ppm' , 0   , 20   , 0.1  , 200   , -750 , -450 , 'nA/ppm', NULL, 't80', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (55 , 4 , 'ppm' , 0   , 200  , 2    , 400   , -150 , -90  , 'nA/ppm', NULL, 't80', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (56 , 4 , 'ppm' , 0   , 20   , 0.1  , NULL  , 200  , 360  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (57 , 5 , 'ppm' , 0   , 5    , 0.1  , 10    , -750 , -450 , 'nA/ppm', NULL, 't80', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (58 , 3 , 'ppm' , 0   , 1000 , 2    , 2800  , 35   , 65   , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (59 , 3 , 'ppm' , 0   , 200  , 0.15 , 500   , 400  , 600  , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (60 , 3 , 'ppm' , 0   , 300  , 1    , NULL  , 35   , 65   , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (61 , 3 , 'ppm' , 0   , 500  , 1    , 2000  , 50   , 90   , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (62 , 3 , 'ppm' , 0   , 500  , 1    , NULL  , 55   , 85   , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (63 , 8 , 'ppm' , 0   , 10   , 0.03 , 20    , 1500 , 2300 , 'nA/ppm', NULL, 't90', 140 , NULL, NULL, NULL);
INSERT INTO Detects VALUES (64 , 8 , 'ppm' , 0   , 100  , 0.2  , 200   , 160  , 240  , 'nA/ppm', NULL, 't90', 140 , NULL, NULL, NULL);
INSERT INTO Detects VALUES (65 , 8 , 'ppm' , 0   , 1000 , 8    , 2000  , 19   , 31   , 'nA/ppm', NULL, 't90', 140 , NULL, NULL, NULL);
INSERT INTO Detects VALUES (66 , 8 , 'ppm' , 0   , 500  , 4    , 1000  , 35   , 65   , 'nA/ppm', NULL, 't90', 140 , NULL, NULL, NULL);
INSERT INTO Detects VALUES (67 , 12, 'ppm' , 0   , 1000 , 1    , 2000  , 15   , 35   , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (68 , 12, 'ppm' , 0   , 4000 , 2    , 8000  , 4    , 16   , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (69 , 12, 'ppm' , 0   , 40000, 10   , 40000 , 2    , 8    , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (70 , 17, 'ppm' , 0   , 100  , 0.3  , 200   , 700  , 1100 , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (71 , 17, 'ppm' , 0   , 500  , 1    , 1000  , 150  , 250  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (72 , 19, 'ppm' , 0   , 100  , 0.1  , NULL  , 560  , 840  , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (73 , 19, 'ppm' , 0   , 200  , 0.2  , NULL  , 200  , 300  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (74 , 19, 'ppm' , 0   , 2000 , 2    , NULL  , 40   , 60   , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (75 , 19, 'ppm' , 0   , 50   , 0.05 , NULL  , 950  , 1450 , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (76 , 19, 'ppm' , 0   , 500  , 1    , NULL  , 80   , 120  , 'nA/ppm', NULL, 't90', 20  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (77 , 14, 'ppm' , 0   , 20   , 0.2  , 40    , 250  , 550  , 'nA/ppm', NULL, 't80', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (78 , 14, 'ppm' , 0   , 200  , 1.5  , 400   , 50   , 110  , 'nA/ppm', NULL, 't80', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (79 , 15, 'ppm' , 0   , 50   , 0.5  , NULL  , 150  , 250  , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (80 , 1 , 'ppm' , 0   , 100  , 2    , NULL  , 40   , 60   , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (81 , 1 , 'ppm' , 0   , 100  , 1    , 200   , 80   , 140  , 'nA/ppm', NULL, 't90', 40  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (82 , 1 , 'ppm' , 0   , 1000 , 8    , NULL  , 12   , 26   , 'nA/ppm', NULL, 't90', 35  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (83 , 1 , 'ppm' , 0   , 1000 , 4    , NULL  , 17   , 33   , 'nA/ppm', NULL, 't90', 35  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (84 , 1 , 'ppm' , 0   , 10000, 40   , NULL  , 1.9  , 3.1  , 'nA/ppm', NULL, 't90', 35  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (85 , 1 , 'ppm' , 0   , 2000 , 4    , NULL  , 8    , 16   , 'nA/ppm', NULL, 't90', 35  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (86 , 1 , 'ppm' , 0   , 500  , 3    , NULL  , 20   , 50   , 'nA/ppm', NULL, 't90', 35  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (87 , 1 , 'ppm' , 0   , 5000 , 10   , NULL  , 3    , 7    , 'nA/ppm', NULL, 't90', 35  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (88 , 20, 'ppm' , 0   , 1000 , 2    , 2000  , 75   , 125  , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (89 , 20, 'ppm' , 0   , 25   , 0.2  , 100   , 600  , 1400 , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (90 , 20, 'ppm' , 0   , 250  , 0.5  , NULL  , 280  , 440  , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (91 , 21, 'ppm' , 0   , 100  , 0.5  , 200   , -150 , -90  , 'nA/ppm', NULL, 't90', 40  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (92 , 21, 'ppm' , 0   , 20   , 0.1  , NULL  , -750 , -450 , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (93 , 21, 'ppm' , 0   , 20   , 0.1  , NULL  , -370 , -230 , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (94 , 21, 'ppm' , 0   , 50   , 0.5  , 100   , 230  , 370  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (96 , 21, 'ppm' , 0   , 500  , 0.5  , 1000  , -130 , -70  , 'nA/ppm', NULL, 't90', 40  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (97 , 22, 'ppm' , 0   , 10000, 50   , 210000, 130  , 130  , 'nA/ppm', NULL, 't90', 10  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (98 , 22, '%Vol', 0   , 30   , 500  , 100   , 56   , 104  , 'μA/ppm', NULL, 't90', 8   , NULL, NULL, NULL);
INSERT INTO Detects VALUES (100, 22, '%Vol', 0   , 30   , 0.05 , 100   , 40   , 80   , 'μA/ppm', NULL, 't90', 15  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (101, 22, '%Vol', 0   , 30   , 0.05 , 100   , 66   , 114  , 'μA/ppm', NULL, 't90', 11  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (102, 23, 'ppm' , 0   , 100  , 0.2  , 200   , -450 , -350 , 'nA/ppm', NULL, 't80', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (104, 23, 'ppm' , 0   , 5    , 0.03 , 50    , -1350, -650 , 'nA/ppm', NULL, 't80', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (105, 24, 'ppm' , 0   , 20   , 0.1  , 100   , 700  , 1300 , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (106, 24, 'ppm' , 0   , 200  , 0.3  , 400   , 240  , 360  , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (107, 24, 'ppm' , 0   , 2000 , 2    , 4000  , 35   , 65   , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (108, 24, 'ppm' , 0   , 5    , 0.03 , 25    , 2500 , 4500 , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (109, 25, 'ppm' , 0   , 50   , 0.3  , NULL  , 440  , 560  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (110, 26, 'ppm' , 0   , 100  , 0.5  , 200   , 220  , 380  , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (111, 26, 'ppm' , 0   , 20   , 0.3  , 100   , 410  , 590  , 'nA/ppm', NULL, 't90', 15  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (112, 26, 'ppm' , 0   , 20   , 0.5  , 100   , 370  , 530  , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (113, 26, 'ppm' , 0   , 100  , 0.2  , 500   , 300  , 440  , 'nA/ppm', NULL, 't90', 20  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (114, 26, 'ppm' , 0   , 1000 , 2    , 5000  , 35   , 65   , 'nA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (115, 26, 'ppm' , 0   , 10000, 2    , 20000 , 15   , 25   , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (116, 26, 'ppm' , 0   , 20   , 0.2  , 100   , 350  , 650  , 'nA/ppm', NULL, 't90', 15  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (117, 26, 'ppm' , 0   , 200  , 0.25 , 600   , 240  , 360  , 'nA/ppm', NULL, 't90', 20  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (118, 26, 'ppm' , 0   , 2000 , 4    , 5000  , 20   , 30   , 'nA/ppm', NULL, 't90', 20  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (119, 2 , 'ppm' , 0   , 0.5  , 0.02 , NULL  , 340  , 800  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, '±10');
INSERT INTO Detects VALUES (120, 2 , 'ppm' , 0.03, 1    , 0.015, 20    , 950  , 1850 , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (121, 2 , 'ppm' , 0.03, 1    , 0.015, 20    , 950  , 1850 , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (122, 6 , 'ppm' , 0.03, 1    , 0.015, 10    , 1700 , 2700 , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (123, 4 , 'ppm' , 0   , 5    , 0.05 , 10    , -650 , -250 , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (124, 4 , 'ppm' , 0   , 5    , 0.05 , 50    , -650 , -250 , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (125, 5 , 'ppm' , 0   , 1    , 0.03 , NULL  , -800 , -400 , 'nA/ppm', NULL, 't90', 120 , NULL, NULL, '±15');
INSERT INTO Detects VALUES (126, 3 , 'ppm' , 0   , 500  , 3    , NULL  , 50   , 90   , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '150');
INSERT INTO Detects VALUES (127, 32, 'ppm' , 0   , 1    , 0.02 , NULL  , 500  , 800  , 'nA/ppm', NULL, 't90', 120 , NULL, NULL, '±10');
INSERT INTO Detects VALUES (128, 33, 'ppm' , 0   , 1    , 0.02 , NULL  , -1300, -700 , 'nA/ppm', NULL, 't90', 80  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (129, 12, 'ppm' , 0   , 10000, 20   , NULL  , 5    , 15   , 'nA/ppm', NULL, 't90', 70  , NULL, NULL, '±250');
INSERT INTO Detects VALUES (130, 12, 'ppm' , 0   , 40000, 100  , NULL  , 0.5  , 1.5  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, '±100');
INSERT INTO Detects VALUES (131, 19, 'ppm' , 0   , 50   , 0.7  , NULL  , 300  , 460  , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±200');
INSERT INTO Detects VALUES (132, 19, 'ppm' , 0   , 100  , 0.3  , NULL  , 600  , 900  , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±200');
INSERT INTO Detects VALUES (133, 19, 'ppm' , 0   , 100  , 0.3  , NULL  , 600  , 900  , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±200');
INSERT INTO Detects VALUES (134, 14, 'ppm' , 0   , 30   , 0.7  , NULL  , 80   , 200  , 'nA/ppm', NULL, 't90', 70  , NULL, NULL, '±100');
INSERT INTO Detects VALUES (134, 13, 'ppm' , 0   , 30   , 0.7  , NULL  , 80   , 200  , 'nA/ppm', NULL, 't90', 70  , NULL, NULL, '±100');
INSERT INTO Detects VALUES (135, 15, 'ppm' , 0   , 30   , 0.2  , NULL  , 15   , 45   , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±5');
INSERT INTO Detects VALUES (136, 15, 'ppm' , 0   , 30   , 0.2  , 100   , 45   , 75   , 'nA/ppm', NULL, 't90', 50  , NULL, NULL, '±15');
INSERT INTO Detects VALUES (137, 16, 'ppm' , 0   , 10   , 0.15 , NULL  , -400 , -200 , 'nA/ppm', NULL, 't90', 90  , NULL, NULL, '±30');
INSERT INTO Detects VALUES (138, 31, 'ppm' , 0   , 1    , 0.01 , NULL  , 900  , 1500 , 'nA/ppm', NULL, 't90', 120 , NULL, NULL, '±15');
INSERT INTO Detects VALUES (139, 1 , 'ppm' , 0   , 100  , 2    , NULL  , 50   , 130  , 'nA/ppm', NULL, 't90', 120 , NULL, NULL, '±150');
INSERT INTO Detects VALUES (140, 1 , 'ppm' , 40  , 1000 , 0.015, 5000  , 3    , 9    , 'nA/ppm', NULL, 't90', 120 , NULL, NULL, '±40');
INSERT INTO Detects VALUES (141, 1 , 'ppm' , 0   , 1000 , 12   , NULL  , 4    , 12   , 'nA/ppm', NULL, 't90', 90  , NULL, NULL, '±40');
INSERT INTO Detects VALUES (142, 1 , 'ppm' , 0   , 100  , 1    , NULL  , 100  , 160  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, '±100');
INSERT INTO Detects VALUES (143, 1 , 'ppm' , 0   , 5000 , 50   , NULL  , 2    , 6    , 'nA/ppm', NULL, 't90', 90  , NULL, NULL, '±100');
INSERT INTO Detects VALUES (144, 1 , 'ppm' , 0   , 500  , NULL , NULL  , 20   , 50   , 'nA/ppm', NULL, 't90', 90  , NULL, NULL, '±100');
INSERT INTO Detects VALUES (145, 21, 'ppm' , 0   , 50   , 0.1  , NULL  , -240 , -160 , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (146, 20, 'ppm' , 0   , 100  , 0.7  , 500   , 30   , 60   , 'nA/ppm', NULL, 't90', 20  , NULL, NULL, '±30');
INSERT INTO Detects VALUES (147, 23, 'ppm' , 0   , 1    , 0.02 , NULL  , -2000, -1000, 'nA/ppm', NULL, 't90', 60  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (148, 23, 'ppm' , 0   , 1    , 0.03 , NULL  , -600 , -300 , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, '±10');
INSERT INTO Detects VALUES (149, 24, 'ppm' , 0.03, 5    , 0.015, 20    , 1500 , 2500 , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (150, 24, 'ppm' , 0.03, 5    , 0.015, 20    , 1700 , 2700 , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (151, 18, 'ppm' , 0.05, 5    , 0.035, 10    , 600  , 1600 , 'nA/ppm', NULL, 't90', 30  , NULL, NULL, '±20');
INSERT INTO Detects VALUES (152, 25, 'ppm' , 1   , 50   , 0.5  , 50    , 60   , 200  , 'nA/ppm', NULL, 't90', 60  , NULL, NULL, '±25');
INSERT INTO Detects VALUES (153, 34, 'ppm' , 0   , 14   , 0.14 , NULL  , 3    , 8    , 'nA/ppm', NULL, 't90', 90  , NULL, NULL, '±4');
INSERT INTO Detects VALUES (154, 3 , 'ppm' , 0   , 1000 , 0.5  , 2000  , 0.015, 0.031, 'μA/ppm', NULL, 't90', 50  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (155, 22, '%Vol', 0   , 25   , NULL , 30    , 0.1  , 0.3  , 'mA/ppm', NULL, 't90', 15  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (156, 10, 'ppm' , 0   , 10   , 0.1  , 100   , 5.8  , 17.8 , 'μA/ppm', NULL, 't90', 90  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (157, 4 , 'ppm' , 0   , 10   , 0.1  , 100   , 0.45 , 0.75 , 'μA/ppm', NULL, 't90', 60  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (158, 3 , 'ppm' , 0   , 1000 , 0.5  , 2000  , 0.055, 0.085, 'μA/ppm', NULL, 't90', 20  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (159, 8 , 'ppm' , 0   , 20   , 0.1  , 100   , 1.5  , 2.1  , 'μA/ppm', NULL, 't90', 120 , NULL, NULL, NULL);
INSERT INTO Detects VALUES (160, 12, 'ppm' , 0   , 1000 , 2    , 2000  , 0.005, 0.015, 'μA/ppm', NULL, 't90', 90  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (161, 19, 'ppm' , 0   , 100  , 0.1  , 500   , 0.65 , 0.95 , 'μA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (162, 14, 'ppm' , 0   , 20   , 0.1  , 200   , 0.4  , 1.2  , 'μA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (163, 16, 'ppm' , 0   , 10   , 0.1  , 100   , 0.25 , 0.55 , 'μA/ppm', NULL, 't90', 90  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (164, 1 , 'ppm' , 0   , 100  , 0.5  , 200   , 0.05 , 0.15 , 'μA/ppm', NULL, 't90', 90  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (165, 21, 'ppm' , 0   , 20   , 0.1  , 150   , 0.36 , 1.2  , 'μA/ppm', NULL, 't90', 25  , NULL, NULL, NULL);
INSERT INTO Detects VALUES (166, 23, 'ppm' , 0   , 20   , 0.2  , 100   , 0.45 , 0.75 , 'μA/ppm', NULL, 't90', 120 , NULL, NULL, NULL);
INSERT INTO Detects VALUES (167, 26, 'ppm' , 0   , 20   , 0.1  , 150   , 0.4  , 0.7  , 'μA/ppm', NULL, 't90', 30  , NULL, NULL, NULL);