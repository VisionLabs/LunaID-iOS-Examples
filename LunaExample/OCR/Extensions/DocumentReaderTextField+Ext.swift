//
//  DocumentReaderTextField+Ext.swift
//  OCR
//
//

import Foundation
import DocumentReader

extension DocumentReaderTextField {
    
    func asField() -> OCRResultTextField? {
        guard let value = getValue() else { return nil }        
        return OCRResultTextField(typeName: fieldType.typeName, localizedTypeName: fieldName, value: value.value)
    }
    
}

extension FieldType {
    
    var typeName: String {
        return raw[rawValue] ?? "Undefined"
    }
    
    private var raw: [Int: String] {
        return [
            0: "Document_Class_Code",
            1: "Issuing_State_Code",
            2: "Document_Number",
            3: "Date_of_Expiry",
            4: "Date_of_Issue",
            5: "Date_of_Birth",
            6: "Place_of_Birth",
            7: "Personal_Number",
            8: "Surname",
            9: "Given_Names",
            10: "Mothers_Name",
            11: "Nationality",
            12: "Sex",
            13: "Height",
            14: "Weight",
            15: "Eyes_Color",
            16: "Hair_Color",
            17: "Address",
            18: "Donor",
            19: "Social_Security_Number",
            20: "DL_Class",
            21: "DL_Endorsed",
            22: "DL_Restriction_Code",
            23: "DL_Under_21_Date",
            24: "Authority",
            25: "Surname_And_Given_Names",
            26: "Nationality_Code",
            27: "Passport_Number",
            28: "Invitation_Number",
            29: "Visa_ID",
            30: "Visa_Class",
            31: "Visa_SubClass",
            32: "MRZ_String1",
            33: "MRZ_String2",
            34: "MRZ_String3",
            35: "MRZ_Type",
            36: "Optional_Data",
            37: "Document_Class_Name",
            38: "Issuing_State_Name",
            39: "Place_of_Issue",
            40: "Document_Number_Checksum",
            41: "Date_of_Birth_Checksum",
            42: "Date_of_Expiry_Checksum",
            43: "Personal_Number_Checksum",
            44: "FinalChecksum",
            45: "Passport_Number_Checksum",
            46: "Invitation_Number_Checksum",
            47: "Visa_ID_Checksum",
            48: "Surname_And_Given_Names_Checksum",
            49: "Visa_Valid_Until_Checksum",
            50: "Other",
            51: "MRZ_Strings",
            52: "Name_Suffix",
            53: "Name_Prefix",
            54: "Date_of_Issue_Checksum",
            55: "Date_of_Issue_CheckDigit",
            56: "Document_Series",
            57: "RegCert_RegNumber",
            58: "RegCert_CarModel",
            59: "RegCert_CarColor",
            60: "RegCert_BodyNumber",
            61: "RegCert_CarType",
            62: "RegCert_MaxWeight",
            63: "Reg_Cert_Weight",
            64: "Address_Area",
            65: "Address_State",
            66: "Address_Building",
            67: "Address_House",
            68: "Address_Flat",
            69: "Place_of_Registration",
            70: "Date_of_Registration",
            71: "Resident_From",
            72: "Resident_Until",
            73: "Authority_Code",
            74: "Place_of_Birth_Area",
            75: "Place_of_Birth_StateCode",
            76: "Address_Street",
            77: "Address_City",
            78: "Address_Jurisdiction_Code",
            79: "Address_Postal_Code",
            80: "Document_Number_CheckDigit",
            81: "Date_of_Birth_CheckDigit",
            82: "Date_of_Expiry_CheckDigit",
            83: "Personal_Number_CheckDigit",
            84: "FinalCheckDigit",
            85: "Passport_Number_CheckDigit",
            86: "Invitation_Number_CheckDigit",
            87: "Visa_ID_CheckDigit",
            88: "Surname_And_Given_Names_CheckDigit",
            89: "Visa_Valid_Until_CheckDigit",
            90: "Permit_DL_Class",
            91: "Permit_Date_of_Expiry",
            92: "Permit_Identifier",
            93: "Permit_Date_of_Issue",
            94: "Permit_Restriction_Code",
            95: "Permit_Endorsed",
            96: "Issue_Timestamp",
            97: "Number_of_Duplicates",
            98: "Medical_Indicator_Codes",
            99: "Non_Resident_Indicator",
            100: "Visa_Type",
            101: "Visa_Valid_From",
            102: "Visa_Valid_Until",
            103: "Duration_of_Stay",
            104: "Number_of_Entries",
            105: "Day",
            106: "Month",
            107: "Year",
            108: "Unique_Customer_Identifier",
            109: "Commercial_Vehicle_Codes",
            110: "AKA_Date_of_Birth",
            111: "AKA_Social_Security_Number",
            112: "AKA_Surname",
            113: "AKA_Given_Names",
            114: "AKA_Name_Suffix",
            115: "AKA_Name_Prefix",
            116: "Mailing_Address_Street",
            117: "Mailing_Address_City",
            118: "Mailing_Address_Jurisdiction_Code",
            119: "Mailing_Address_Postal_Code",
            120: "Audit_Information",
            121: "Inventory_Number",
            122: "Race_Ethnicity",
            123: "Jurisdiction_Vehicle_Class",
            124: "Jurisdiction_Endorsement_Code",
            125: "Jurisdiction_Restriction_Code",
            126: "Family_Name",
            127: "Given_Names_RUS",
            128: "Visa_ID_RUS",
            129: "Fathers_Name",
            130: "Fathers_Name_RUS",
            131: "Surname_And_Given_Names_RUS",
            132: "Place_Of_Birth_RUS",
            133: "Authority_RUS",
            134: "Issuing_State_Code_Numeric",
            135: "Nationality_Code_Numeric",
            136: "Engine_Power",
            137: "Engine_Volume",
            138: "Chassis_Number",
            139: "Engine_Number",
            140: "Engine_Model",
            141: "Vehicle_Category",
            142: "Identity_Card_Number",
            143: "Control_No",
            144: "Parrent_s_Given_Names",
            145: "Second_Surname",
            146: "Middle_Name",
            147: "RegCert_VIN",
            148: "RegCert_VIN_CheckDigit",
            149: "RegCert_VIN_Checksum",
            150: "Line1_CheckDigit",
            151: "Line2_CheckDigit",
            152: "Line3_CheckDigit",
            153: "Line1_Checksum",
            154: "Line2_Checksum",
            155: "Line3_Checksum",
            156: "RegCert_RegNumber_CheckDigit",
            157: "RegCert_RegNumber_Checksum",
            158: "RegCert_Vehicle_ITS_Code",
            159: "Card_Access_Number",
            160: "Marital_Status",
            161: "Company_Name",
            162: "Special_Notes",
            163: "Surname_of_Spose",
            164: "Tracking_Number",
            165: "Booklet_Number",
            166: "Children",
            167: "Copy",
            168: "Serial_Number",
            169: "Dossier_Number",
            170: "AKA_Surname_And_Given_Names",
            171: "Territorial_Validity",
            172: "MRZ_Strings_With_Correct_CheckSums",
            173: "DL_CDL_Restriction_Code",
            174: "DL_Under_18_Date",
            175: "DL_Record_Created",
            176: "DL_Duplicate_Date",
            177: "DL_Iss_Type",
            178: "Military_Book_Number",
            179: "Destination",
            180: "Blood_Group",
            181: "Sequence_Number",
            182: "RegCert_BodyType",
            183: "RegCert_CarMark",
            184: "Transaction_Number",
            185: "Age",
            186: "Folio_Number",
            187: "Voter_Key",
            188: "Address_Municipality",
            189: "Address_Location",
            190: "Section",
            191: "OCR_Number",
            192: "Federal_Elections",
            193: "Reference_Number",
            194: "Optional_Data_Checksum",
            195: "Optional_Data_CheckDigit",
            196: "Visa_Number",
            197: "Visa_Number_Checksum",
            198: "Visa_Number_CheckDigit",
            199: "Voter",
            200: "Previous_Type",
            220: "FieldFromMRZ",
            221: "CurrentDate",
            251: "Status_Date_of_Expiry",
            252: "Banknote_Number",
            253: "CSC_Code",
            254: "Artistic_Name",
            255: "Academic_Title",
            256: "Address_Country",
            257: "Address_Zipcode",
            258: "EID_Residence_Permit1",
            259: "EID_Residence_Permit2",
            260: "EID_PlaceOfBirth_Street",
            261: "EID_PlaceOfBirth_City",
            262: "EID_PlaceOfBirth_State",
            263: "EID_PlaceOfBirth_Country",
            264: "EID_PlaceOfBirth_Zipcode",
            265: "CDL_Class",
            266: "DL_Under_19_Date",
            267: "Weight_pounds",
            268: "Limited_Duration_Document_Indicator",
            269: "Endorsement_Expiration_Date",
            270: "Revision_Date",
            271: "Compliance_Type",
            272: "Family_name_truncation",
            273: "First_name_truncation",
            274: "Middle_name_truncation",
            275: "Exam_Date",
            276: "Organization",
            277: "Department",
            278: "Pay_Grade",
            279: "Rank",
            280: "Benefits_Number",
            281: "Sponsor_Service",
            282: "Sponsor_Status",
            283: "Sponsor",
            284: "Relationship",
            285: "USCIS",
            286: "Category",
            287: "Conditions",
            288: "Identifier",
            289: "Configuration",
            290: "Discretionary_data",
            291: "Line1_Optional_Data",
            292: "Line2_Optional_Data",
            293: "Line3_Optional_Data",
            294: "EQV_Code",
            295: "ALT_Code",
            296: "Binary_Code",
            297: "Pseudo_Code",
            298: "Fee",
            299: "Stamp_Number",
            300: "SBH_SecurityOptions",
            301: "SBH_IntegrityOptions",
            302: "Date_of_Creation",
            303: "Validity_Period",
            304: "Patron_Header_Version",
            305: "BDB_Type",
            306: "Biometric_Type",
            307: "Biometric_Subtype",
            308: "Biometric_ProductID",
            309: "Biometric_Format_Owner",
            310: "Biometric_Format_Type",
            311: "Phone",
            312: "Profession",
            313: "Title",
            314: "Personal_Summary",
            315: "Other_Valid_ID",
            316: "Custody_Info",
            317: "Other_Name",
            318: "Observations",
            319: "Tax",
            320: "Date_of_Personalization",
            321: "Personalization_SN",
            322: "OtherPerson_Name",
            323: "PersonToNotify_Date_of_Record",
            324: "PersonToNotify_Name",
            325: "PersonToNotify_Phone",
            326: "PersonToNotify_Address",
            327: "DS_Certificate_Issuer",
            328: "DS_Certificate_Subject",
            329: "DS_Certificate_ValidFrom",
            330: "DS_Certificate_ValidTo",
            331: "VRC_DataObject_Entry",
            332: "TypeApprovalNumber",
            333: "AdministrativeNumber",
            334: "DocumentDiscriminator",
            335: "DataDiscriminator",
            336: "ISO_Issuer_ID_Number",
            340: "GNIB_Number",
            341: "Dept_Number",
            342: "Telex_Code",
            343: "Allergies",
            344: "Sp_Code",
            345: "Court_Code",
            346: "Cty",
            347: "Sponsor_SSN",
            348: "DoD_Number",
            349: "MC_Novice_Date",
            350: "DUF_Number",
            351: "AGY",
            352: "PNR_Code",
            353: "From_Airport_Code",
            354: "To_Airport_Code",
            355: "Flight_Number",
            356: "Date_of_Flight",
            357: "Seat_Number",
            358: "Date_of_Issue_Boarding_Pass",
            359: "CCW_Until",
            360: "Reference_Number_Checksum",
            361: "Reference_Number_CheckDigit",
            362: "Room_Number",
            363: "Religion",
            364: "RemainderTerm",
            365: "Electronic_Ticket_Indicator",
            366: "Compartment_Code",
            367: "CheckIn_Sequence_Number",
            368: "Airline_Designator_of_boarding_pass_issuer",
            369: "Airline_Numeric_Code",
            370: "Ticket_Number",
            371: "Frequent_Flyer_Airline_Designator",
            372: "Frequent_Flyer_Number",
            373: "Free_Baggage_Allowance",
            374: "PDF417Codec",
            375: "Identity_Card_Number_Checksum",
            376: "Identity_Card_Number_CheckDigit",
            377: "Veteran",
            378: "DLClassCode_A1_From",
            379: "DLClassCode_A1_To",
            380: "DLClassCode_A1_Notes",
            381: "DLClassCode_A_From",
            382: "DLClassCode_A_To",
            383: "DLClassCode_A_Notes",
            384: "DLClassCode_B_From",
            385: "DLClassCode_B_To",
            386: "DLClassCode_B_Notes",
            387: "DLClassCode_C1_From",
            388: "DLClassCode_C1_To",
            389: "DLClassCode_C1_Notes",
            390: "DLClassCode_C_From",
            391: "DLClassCode_C_To",
            392: "DLClassCode_C_Notes",
            393: "DLClassCode_D1_From",
            394: "DLClassCode_D1_To",
            395: "DLClassCode_D1_Notes",
            396: "DLClassCode_D_From",
            397: "DLClassCode_D_To",
            398: "DLClassCode_D_Notes",
            399: "DLClassCode_BE_From",
            400: "DLClassCode_BE_To",
            401: "DLClassCode_BE_Notes",
            402: "DLClassCode_C1E_From",
            403: "DLClassCode_C1E_To",
            404: "DLClassCode_C1E_Notes",
            405: "DLClassCode_CE_From",
            406: "DLClassCode_CE_To",
            407: "DLClassCode_CE_Notes",
            408: "DLClassCode_D1E_From",
            409: "DLClassCode_D1E_To",
            410: "DLClassCode_D1E_Notes",
            411: "DLClassCode_DE_From",
            412: "DLClassCode_DE_To",
            413: "DLClassCode_DE_Notes",
            414: "DLClassCode_M_From",
            415: "DLClassCode_M_To",
            416: "DLClassCode_M_Notes",
            417: "DLClassCode_L_From",
            418: "DLClassCode_L_To",
            419: "DLClassCode_L_Notes",
            420: "DLClassCode_T_From",
            421: "DLClassCode_T_To",
            422: "DLClassCode_T_Notes",
            423: "DLClassCode_AM_From",
            424: "DLClassCode_AM_To",
            425: "DLClassCode_AM_Notes",
            426: "DLClassCode_A2_From",
            427: "DLClassCode_A2_To",
            428: "DLClassCode_A2_Notes",
            429: "DLClassCode_B1_From",
            430: "DLClassCode_B1_To",
            431: "DLClassCode_B1_Notes",
            432: "Surname_at_Birth",
            433: "Civil_Status",
            434: "Number_of_Seats",
            435: "Number_of_Standing_Places",
            436: "Max_Speed",
            437: "Fuel_Type",
            438: "EC_Environmental_Type",
            439: "Power_Weight_Ratio",
            440: "Max_Mass_of_Trailer_Braked",
            441: "Max_Mass_of_Trailer_Unbraked",
            442: "Transmission_Type",
            443: "Trailer_Hitch",
            444: "Accompanied_by",
            445: "Police_District",
            446: "First_Issue_Date",
            447: "Payload_Capacity",
            448: "Number_of_Axels",
            449: "Permissible_Axle_Load",
            450: "Precinct",
            451: "Invited_by",
            452: "Purpose_of_Entry",
            453: "Skin_Color",
            454: "Complexion",
            455: "Airport_From",
            456: "Airport_To",
            457: "Airline_Name",
            458: "Airline_Name_Frequent_Flyer",
            459: "License_Number",
            460: "In_Tanks",
            461: "Exept_In_Tanks",
            462: "Fast_Track",
            463: "Owner",
            464: "MRZ_Strings_ICAO_RFID",
            465: "Number_of_Card_Issuance",
            466: "Number_of_Card_Issuance_Checksum",
            467: "Number_of_Card_Issuance_CheckDigit",
            468: "Century_Date_of_Birth",
            469: "DLClassCode_A3_From",
            470: "DLClassCode_A3_To",
            471: "DLClassCode_A3_Notes",
            472: "DLClassCode_C2_From",
            473: "DLClassCode_C2_To",
            474: "DLClassCode_C2_Notes",
            475: "DLClassCode_B2_From",
            476: "DLClassCode_B2_To",
            477: "DLClassCode_B2_Notes",
            478: "DLClassCode_D2_From",
            479: "DLClassCode_D2_To",
            480: "DLClassCode_D2_Notes",
            481: "DLClassCode_B2E_From",
            482: "DLClassCode_B2E_To",
            483: "DLClassCode_B2E_Notes",
            484: "DLClassCode_G_From",
            485: "DLClassCode_G_To",
            486: "DLClassCode_G_Notes",
            487: "DLClassCode_J_From",
            488: "DLClassCode_J_To",
            489: "DLClassCode_J_Notes",
            490: "DLClassCode_LC_From",
            491: "DLClassCode_LC_To",
            492: "DLClassCode_LC_Notes",
            493: "BankCardNumber",
            494: "BankCardValidThru",
            495: "TaxNumber",
            496: "HealthNumber",
            497: "GrandfatherName",
            498: "Selectee_Indicator",
            499: "Mother_Surname",
            500: "Mother_GivenName",
            501: "Father_Surname",
            502: "Father_GivenName",
            503: "Mother_DateOfBirth",
            504: "Father_DateOfBirth",
            505: "Mother_PersonalNumber",
            506: "Father_PersonalNumber",
            507: "Mother_PlaceOfBirth",
            508: "Father_PlaceOfBirth",
            509: "Mother_CountryOfBirth",
            510: "Father_CountryOfBirth",
            511: "Date_First_Renewal",
            512: "Date_Second_Renewal",
            513: "PlaceOfExamination",
            514: "ApplicationNumber",
            515: "VoucherNumber",
            516: "AuthorizationNumber",
            517: "Faculty",
            518: "FormOfEducation",
            519: "DNINumber",
            520: "RetirementNumber",
            521: "ProfessionalIdNumber",
            522: "Age_at_Issue",
            523: "Years_Since_Issue",
            524: "DLClassCode_BTP_From",
            525: "DLClassCode_BTP_Notes",
            526: "DLClassCode_BTP_To",
            527: "DLClassCode_C3_From",
            528: "DLClassCode_C3_Notes",
            529: "DLClassCode_C3_To",
            530: "DLClassCode_E_From",
            531: "DLClassCode_E_Notes",
            532: "DLClassCode_E_To",
            533: "DLClassCode_F_From",
            534: "DLClassCode_F_Notes",
            535: "DLClassCode_F_To",
            536: "DLClassCode_FA_From",
            537: "DLClassCode_FA_Notes",
            538: "DLClassCode_FA_To",
            539: "DLClassCode_FA1_From",
            540: "DLClassCode_FA1_Notes",
            541: "DLClassCode_FA1_To",
            542: "DLClassCode_FB_From",
            543: "DLClassCode_FB_Notes",
            544: "DLClassCode_FB_To",
            545: "DLClassCode_G1_From",
            546: "DLClassCode_G1_Notes",
            547: "DLClassCode_G1_To",
            548: "DLClassCode_H_From",
            549: "DLClassCode_H_Notes",
            550: "DLClassCode_H_To",
            551: "DLClassCode_I_From",
            552: "DLClassCode_I_Notes",
            553: "DLClassCode_I_To",
            554: "DLClassCode_K_From",
            555: "DLClassCode_K_Notes",
            556: "DLClassCode_K_To",
            557: "DLClassCode_LK_From",
            558: "DLClassCode_LK_Notes",
            559: "DLClassCode_LK_To",
            560: "DLClassCode_N_From",
            561: "DLClassCode_N_Notes",
            562: "DLClassCode_N_To",
            563: "DLClassCode_S_From",
            564: "DLClassCode_S_Notes",
            565: "DLClassCode_S_To",
            566: "DLClassCode_TB_From",
            567: "DLClassCode_TB_Notes",
            568: "DLClassCode_TB_To",
            569: "DLClassCode_TM_From",
            570: "DLClassCode_TM_Notes",
            571: "DLClassCode_TM_To",
            572: "DLClassCode_TR_From",
            573: "DLClassCode_TR_Notes",
            574: "DLClassCode_TR_To",
            575: "DLClassCode_TV_From",
            576: "DLClassCode_TV_Notes",
            577: "DLClassCode_TV_To",
            578: "DLClassCode_V_From",
            579: "DLClassCode_V_Notes",
            580: "DLClassCode_V_To",
            581: "DLClassCode_W_From",
            582: "DLClassCode_W_Notes",
            583: "DLClassCode_W_To",
            584: "URL",
            585: "Caliber",
            586: "Model",
            587: "Make",
            588: "NumberOfCylinders",
            589: "SurnameOfHusbandAfterRegistration",
            590: "SurnameOfWifeAfterRegistration",
            591: "DateOfBirthOfWife",
            592: "DateOfBirthOfHusband",
            593: "CitizenshipOfFirstPerson",
            594: "CitizenshipOfSecondPerson",
            595: "CVV",
            596: "Date_of_Insurance_Expiry",
            597: "Mortgage_by",
            598: "Old_Document_Number",
            599: "Old_Date_of_Issue",
            600: "Old_Place_of_Issue",
            601: "DLClassCode_LR_From",
            602: "DLClassCode_LR_To",
            603: "DLClassCode_LR_Notes",
            604: "DLClassCode_MR_From",
            605: "DLClassCode_MR_To",
            606: "DLClassCode_MR_Notes",
            607: "DLClassCode_HR_From",
            608: "DLClassCode_HR_To",
            609: "DLClassCode_HR_Notes",
            610: "DLClassCode_HC_From",
            611: "DLClassCode_HC_To",
            612: "DLClassCode_HC_Notes",
            613: "DLClassCode_MC_From",
            614: "DLClassCode_MC_To",
            615: "DLClassCode_MC_Notes",
            616: "DLClassCode_RE_From",
            617: "DLClassCode_RE_To",
            618: "DLClassCode_RE_Notes",
            619: "DLClassCode_R_From",
            620: "DLClassCode_R_To",
            621: "DLClassCode_R_Notes",
            622: "DLClassCode_CA_From",
            623: "DLClassCode_CA_To",
            624: "DLClassCode_CA_Notes"
        ]
    }
}

extension DiDocType {
    
    var typeName: String {
        return raw[rawValue] ?? "Undefined"
    }
    
    private var raw: [Int: String] {
        return [0: "NotDefined",
                11: "Passport",
                12: "IdentityCard",
                13: "DiplomaticPassport",
                14: "ServicePassport",
                15: "SeamansIdentityDocument",
                16: "IdentityCardforResidence",
                17: "Traveldocument",
                99: "Other",
                29: "VisaID2",
                30: "VisaID3",
                31: "RegistrationCertificate",
                20: "NationalIdentityCard",
                21: "SocialIdentityCard",
                22: "AliensIdentityCard",
                23: "PrivilegedIdentityCard",
                24: "ResidencePermitIdentityCard",
                25: "OriginCard",
                26: "EmergencyPassport",
                27: "AliensPassport",
                28: "AlternativeIdentityCard",
                32: "AuthorizationCard",
                33: "BeginnerPermit",
                34: "BorderCrossingCard",
                35: "ChauffeurLicense",
                36: "ChauffeurLicenseUnder18",
                37: "ChauffeurLicenseUnder21",
                38: "CommercialDrivingLicense",
                39: "CommercialDrivingLicenseIndtuctionalPermit",
                40: "CommercialDrivingLicenseUnder18",
                41: "CommercialDrivingLicenseUnder21",
                42: "CommercialIndtuctionPermit",
                43: "CommercialNewPermit",
                44: "ConcealedCarryLicense",
                45: "ConcealedFirearmPermit",
                46: "ConditionalDrivingLicense",
                47: "DepartmentOfVeteransAffairsIdentityCard",
                48: "DiplomaticDrivingLicense",
                49: "DrivingLicense",
                50: "DrivingLicenseIndtuctionalPermit",
                51: "DrivingLicenseIndtuctionalPermitUnder18",
                52: "DrivingLicenseIndtuctionalPermitUnder21",
                53: "DrivingLicenseLearnersPermit",
                54: "DrivingLicenseLearnersPermitUnder18",
                55: "DrivingLicenseLearnersPermitUnder21",
                56: "DrivingLicenseNovice",
                57: "DrivingLicenseNoviceUnder18",
                58: "DrivingLicenseNoviceUnder21",
                59: "DrivingLicenseRegisteredOffender",
                60: "DrivingLicenseRedtictedUnder18",
                61: "DrivingLicenseRedtictedUnder21",
                62: "DrivingLicenseTemporaryVisitor",
                63: "DrivingLicenseTemporaryVisitorUnder18",
                64: "DrivingLicenseTemporaryVisitorUnder21",
                65: "DrivingLicenseUnder18",
                66: "DrivingLicenseUnder21",
                67: "EmploymentDrivingPermit",
                68: "EnhancedChauffeurLicense",
                69: "EnhancedChauffeurLicenseUnder18",
                70: "EnhancedChauffeurLicenseUnder21",
                71: "EnhancedCommercialDrivingLicense",
                72: "EnhancedDrivingLicense",
                73: "EnhancedDrivingLicenseUnder18",
                74: "EnhancedDrivingLicenseUnder21",
                75: "EnhancedIdentityCard",
                76: "EnhancedIdentityCardUnder18",
                77: "EnhancedIdentityCardUnder21",
                78: "EnhancedOperatorsLicense",
                79: "FirearmsPermit",
                80: "FullProvisionalLicense",
                81: "FullProvisionalLicenseUnder18",
                82: "FullProvisionalLicenseUnder21",
                83: "GenevaConventionsIdentityCard",
                84: "GraduatedDrivingLicenseUnder18",
                85: "GraduatedDrivingLicenseUnder21",
                86: "GraduatedIndtuctionPermitUnder18",
                87: "GraduatedIndtuctionPermitUnder21",
                88: "GraduatedLicenseUnder18",
                89: "GraduatedLicenseUnder21",
                90: "HandgunCarryPermit",
                91: "IdentityAndPrivilegeCard",
                92: "IdentityCardMobilityImpaired",
                93: "IdentityCardRegisteredOffender",
                94: "IdentityCardTemporaryVisitor",
                95: "IdentityCardTemporaryVisitorUnder18",
                96: "IdentityCardTemporaryVisitorUnder21",
                97: "IdentityCardUnder18",
                98: "IdentityCardUnder21",
                100: "IgnitionInterlockPermit",
                101: "ImmigrantVisa",
                102: "IndtuctionPermit",
                103: "IndtuctionPermitUnder18",
                104: "IndtuctionPermitUnder21",
                105: "InterimDrivingLicense",
                106: "InterimIdentityCard",
                107: "IntermediateDrivingLicense",
                108: "IntermediateDrivingLicenseUnder18",
                109: "IntermediateDrivingLicenseUnder21",
                110: "JuniorDrivingLicense",
                111: "LearnerIndtuctionalPermit",
                112: "LearnerLicense",
                113: "LearnerLicenseUnder18",
                114: "LearnerLicenseUnder21",
                115: "LearnerPermit",
                116: "LearnerPermitUnder18",
                117: "LearnerPermitUnder21",
                118: "LimitedLicense",
                119: "LimitedPermit",
                120: "LimitedTermDrivingLicense",
                121: "LimitedTermIdentityCard",
                122: "LiquorIdentityCard",
                123: "NewPermit",
                124: "NewPermitUnder18",
                125: "NewPermitUnder21",
                126: "NonUsCitizenDrivingLicense",
                127: "OccupationalDrivingLicense",
                128: "OneidaTribeOfIndiansIdentityCard",
                129: "OperatorLicense",
                130: "OperatorLicenseUnder18",
                131: "OperatorLicenseUnder21",
                132: "PermanentDrivingLicense",
                133: "PermitToReEnter",
                134: "ProbationaryAutoLicense",
                135: "ProbationaryDrivingLicenseUnder18",
                136: "ProbationaryDrivingLicenseUnder21",
                137: "ProbationaryVehicleSalespersonLicense",
                138: "ProvisionalDrivingLicense",
                139: "ProvisionalDrivingLicenseUnder18",
                140: "ProvisionalDrivingLicenseUnder21",
                141: "ProvisionalLicense",
                142: "ProvisionalLicenseUnder18",
                143: "ProvisionalLicenseUnder21",
                144: "PublicPassengerChauffeurLicense",
                145: "RacingAndGamingComissionCard",
                146: "RefugeeTravelDocument",
                147: "RenewalPermit",
                148: "RedtictedCommercialDrivingLicense",
                149: "RedtictedDrivingLicense",
                150: "RedtictedPermit",
                151: "SeasonalPermit",
                152: "SeasonalResidentIdentityCard",
                153: "SeniorCitizenIdentityCard",
                154: "SexOffender",
                155: "SocialSecurityCard",
                156: "TemporaryDrivingLicense",
                157: "TemporaryDrivingLicenseUnder18",
                158: "TemporaryDrivingLicenseUnder21",
                159: "TemporaryIdentityCard",
                160: "TemporaryIndtuctionPermitIdentityCard",
                161: "TemporaryIndtuctionPermitIdentityCardUnder18",
                162: "TemporaryIndtuctionPermitIdentityCardUnder21",
                163: "TemporaryVisitorDrivingLicense",
                164: "TemporaryVisitorDrivingLicenseUnder18",
                165: "TemporaryVisitorDrivingLicenseUnder21",
                166: "UniformedServicesIdentityCard",
                167: "VehicleSalespersonLicense",
                168: "WorkerIdentificationCredential",
                169: "CommercialDrivingLicenseNovice",
                170: "CommercialDrivingLicenseNoviceUnder18",
                171: "CommercialDrivingLicenseNoviceUnder21",
                172: "PassportCard",
                173: "PermanentResidentCard",
                174: "PersonalIdentificationVerification",
                175: "TemporaryOperatorLicense",
                176: "DrivingLicenseUnder19",
                177: "IdentityCardUnder19",
                178: "Visa",
                179: "TemporaryPassport",
                180: "VotingCard",
                181: "HealthCard",
                182: "CertificateOfCitizenship",
                183: "AddressCard",
                184: "AirportImmigrationCard",
                185: "AlienRegidtationCard",
                186: "APEHCard",
                187: "CoupontoDrivingLicense",
                188: "CrewMemberCertificate",
                189: "DocumentForReturn",
                190: "ECard",
                191: "EmploymentCard",
                192: "HKSARImmigrationForm",
                193: "Immigrantcard",
                194: "LabourCard",
                195: "LaissezPasser",
                196: "LawyerIdentityCertificate",
                197: "LicenseCard",
                198: "PassportStateless",
                199: "PassportChild",
                200: "PassportConsular",
                201: "PassportDiplomaticService",
                202: "PassportOfficial",
                203: "PassportProvisional",
                204: "PassportSpecial",
                205: "PermissiontotheLocalBorderTraffic",
                207: "SEDESOLCard",
                208: "SocialCard",
                209: "TBCard",
                210: "VehiclePassport",
                211: "WDocument",
                212: "DiplomaticIdentityCard",
                213: "ConsularIdentityCard",
                214: "IncomeTaxCard",
                215: "ResidencePermit",
                216: "DocumentOfIdentity",
                217: "BorderCrossingPermit",
                218: "PassportLimitedValidity",
                219: "SIMCard",
                220: "TaxCard",
                221: "CompanyCard",
                222: "DomesticPassport",
                223: "IdentityCertificate",
                224: "ResidentIdCard",
                225: "ArmedForcesIdentityCard",
                226: "ProfessionalCard",
                227: "RegistrationStamp",
                228: "DriverCard",
                229: "DriverTrainingCertificate",
                230: "QualificationDrivingLicense",
                231: "MembershipCard",
                232: "PublicVehicleDriverAuthorityCard",
                233: "MarineLicense",
                234: "TemporaryLearnerDrivingLicense",
                235: "TemporaryCommercialDrivingLicense",
                236: "InterimInstructionalPermit",
                237: "CertificateOfCompetency",
                238: "CertificateOfProficiency"]
    }
}