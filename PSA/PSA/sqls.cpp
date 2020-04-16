#include"sqls.h"



c_sql::c_sql()
{
	sql_info xx;
	xx.name = "PORTAL";
	xx.sql = "SELECT * FROM GETVEHSSCREEN(3, '''AP'', ''AC'', ''PP2000'', ''LEXIA'', ''ADF'', ''ADS''', '''RA''', '0'); ";
	sql_map[st_veh_info] = xx;


	xx.name = "PORTAL";
	xx.sql = "SELECT * FROM GETMODARCS (x_x); ";
	sql_map[st_veh_comtype] = xx;


	xx.name = "REF_FAM_VEH";
	xx.sql = "SELECT p.FAMILYNAME, p.FAMILYLABEL, p.OPT, p.PATH, p.FILENAME, p.ICONOK, p.ICONKO, p.UNDEFINE FROM GET_ALL_REF_DATA('x_x', 'Breakdown_research') p";;
	sql_map[st_ecu_info] = xx;


	xx.name = "GPC";
	xx.sql = "SELECT * FROM vehicule WHERE (VEHCOMTYPE = 'x_x') ";
	sql_map[st_veh_id] = xx;


	xx.name = "GPC";
	xx.sql = "select * from RECO r where r.ECUID=x_x and r.RECECUMNEMONAME='x_x' ";
	sql_map[st_ecu_descript_info] = xx;


	xx.name = "GPC";
	xx.sql = "SELECT * FROM family, famtype WHERE ((VEHID = x_x) AND (family.FAMTYID = famtype.FAMTYID)) And famtype.FAMNAME = 'x_x'";
	sql_map[st_ecu_protcol] = xx;

	
	xx.name = "GPC";
	xx.sql = "SELECT p.ECUNAME, p.VARIANTES, p.ECUID FROM GET_VARIANTES_LIST('x_x') p ";
	sql_map[st_ecu_variant_list] = xx;

	
	xx.name = "DSD";
	xx.sql = "SELECT p.ECUNAME, p.ECUVEFILENAME, p.ECUVEID, p.VARIANTENAME, p.REQUESTRECOFRAME, p.ANSWEROKRECOFRAME, p.ANSWERKORECOFRAME, p.REQUESTINITFRAME, p.ANSWEROKINITFRAME, p.ANSWERKOINITFRAME, p.REQUESTFINDIAGFRAME, p.ANSWEROKFINDIAGFRAME, p.ANSWERKOFINDIAGFRAME, p.NBR_RECO_TOTAL, p.NBR_RECO_REQUEST, p.PARSNAME, p.IDEMNEMOVALUE, p.ISPBYTEPOS, p.ISPBYTELENGTH FROM GET_ALL_RECO_INFO('x_x') p";
	sql_map[st_ecu_variant_list_all] = xx;

	
	xx.name = "GPC";
	xx.sql = "select * from screen where screen.ecuid=x_x and screen.scrname = 'x_x' ";
	sql_map[st_ecu_identification] = xx;

	
	xx.name = "GPC";
	xx.sql = "SELECT PARAM.*, I_SCRPAR.SCRID, I_SCRPAR.PARPOS  FROM PARAM, I_SCRPAR  WHERE (I_SCRPAR.SCRID = x_x)  AND I_SCRPAR.PARID = PARAM.PARID  ORDER BY I_SCRPAR.PARPOS ASC ";
	sql_map[st_id_data_stream] = xx;


	xx.name = "DSD";
	xx.sql = "SELECT su.SERUNID, p.PARSNAME FROM I_ECUSER ie INNER JOIN SERVICE s ON ie.SERID = s.SERID INNER JOIN SERVUNIT su ON s.SERID = su.SERID INNER JOIN SERVUNITFRAME sf ON su.SERUNID = sf.SERUNID INNER JOIN I_SERPAR isp ON sf.SERUNFRID = isp.SERUNFRID INNER JOIN PARAM p ON isp.PARID = p.PARID WHERE ie.ECUVEID='x_x' AND sf.SERUNFRTYID=1 AND s.SERSNAME IN ('RDBLID', 'RECUID', 'MODE01', 'MODEA4', 'RDBI', 'SRBLID') AND p.PARSNAME IN ('x_x') AND (isp.PARID IS NOT NULL OR isp.CRCID IS NOT NULL OR isp.MAPID IS NOT NULL) ";
	sql_map[st_data_stream_serun_id] = xx;


	xx.name = "DSD";
	xx.sql = "SELECT distinct sf.SERUNFRTYID, su.SERUNID , sf.SERUNFRDESCRIPTION, sf.SERUNFRID FROM SERVUNIT su inner join SERVUNITFRAME sf on su.SERUNID = sf.SERUNID where su.SERUNID = x_x ";
	sql_map[st_data_stream_serunfr_id] = xx;

	
	xx.name = "DSD";
	xx.sql = "SELECT isp.SERUNFRID, isp.PARID, isp.ISPBYTEPOS, isp.ISPVALUE, isp.BLOID, pt.PARTYNAME, pt.PARTYID, p.PARSNAME, p.PARLNAME, p.PARDESCRIPTION, p.PARENCODING, p.PARTYPE, p.PARBASEENCODING, p.PARINTERNALENCODING, p.PARPHYSICALENCODING, p.PARCOMPUTMETHOD, dt.DATTYID, dt.DATTYNAME, ad.ADDABSOLUTENUMBER, ad.ADDBYTELENGTH, ad.ADDBYTEORDER, ad.ADDBITLENGTH, ad.ADDBITMASK, ad.ADDBITCONDENSED, ad.ADDMAXLENGTH, ad.ADDMINLENGTH, ad.ADDTERMINATION, ad.ADDBITPARAMREF, adt.ADDTYNAME, adt.ADDTYID, c.CRCSNAME, c.CRCLNAME, c.CRCDESCRIPTION, c.CRCFIRSTBYTE, c.CRCLASTBYTE, c.CRCPOLYNOM, c.CRCBYTELENGTH, c.CRCBLOCKNAME, bl.BLOSNAME, bl.BLODESCRIPTION, bl.BLOBYTEPOS, bl.BLOMINOCCURS, bl.BLOMAXOCCURS, bl.BLOBYTELENGTH, bl.ENDOFPDU, mp.MAPSNAME, mp.MAPLNAME, mp.MAPDESCRIPTION, mp.MAPLID, mp.MAPID, db.DYNSNAME, db.DYNBYTEPOS, db.DYNID, db.DYNBYTELENGTH FROM PARTYPE pt INNER JOIN I_SERPAR isp ON isp.PARTYID = pt.PARTYID LEFT OUTER JOIN (PARAM p  INNER JOIN DATATYPE dt ON p.DATTYID = dt.DATTYID) ON isp.PARID = p.PARID LEFT OUTER JOIN ADDDATA ad ON isp.PARID = ad.PARID LEFT OUTER JOIN ADDTYPE adt ON ad.ADDTYID = adt.ADDTYID LEFT OUTER JOIN BLOCK bl ON isp.BLOID = bl.BLOID LEFT OUTER JOIN DYNAMICBLOCK db ON isp.DYNID = db.DYNID LEFT OUTER JOIN CRC c ON isp.CRCID = c.CRCID LEFT OUTER JOIN MAPPED mp ON isp.MAPID = mp.MAPID WHERE (isp.SERUNFRID = x_x) and ((isp.PARID is not null) OR (isp.CRCID is not null) OR (isp.MAPID is not null)) ";
	sql_map[st_data_stream_request_info] = xx;

	xx.name = "DSD";
	xx.sql = "SELECT isp.SERUNFRID, isp.PARID, isp.ISPBYTEPOS, isp.ISPVALUE, isp.BLOID, pt.PARTYNAME, pt.PARTYID, p.PARSNAME, p.PARLNAME, p.PARDESCRIPTION, p.PARENCODING, p.PARTYPE, p.PARBASEENCODING, p.PARINTERNALENCODING, p.PARPHYSICALENCODING, p.PARCOMPUTMETHOD, dt.DATTYID, dt.DATTYNAME, ad.ADDABSOLUTENUMBER, ad.ADDBYTELENGTH, ad.ADDBYTEORDER, ad.ADDBITLENGTH, ad.ADDBITMASK, ad.ADDBITCONDENSED, ad.ADDMAXLENGTH, ad.ADDMINLENGTH, ad.ADDTERMINATION, ad.ADDBITPARAMREF, adt.ADDTYNAME, adt.ADDTYID, c.CRCSNAME, c.CRCLNAME, c.CRCDESCRIPTION, c.CRCFIRSTBYTE, c.CRCLASTBYTE, c.CRCPOLYNOM, c.CRCBYTELENGTH, c.CRCBLOCKNAME, bl.BLOSNAME, bl.BLODESCRIPTION, bl.BLOBYTEPOS, bl.BLOMINOCCURS, bl.BLOMAXOCCURS, bl.BLOBYTELENGTH, bl.ENDOFPDU, mp.MAPSNAME, mp.MAPLNAME, mp.MAPDESCRIPTION, mp.MAPLID, mp.MAPID, db.DYNSNAME, db.DYNBYTEPOS, db.DYNID, db.DYNBYTELENGTH FROM PARTYPE pt INNER JOIN I_SERPAR isp ON isp.PARTYID = pt.PARTYID LEFT OUTER JOIN (PARAM p  INNER JOIN DATATYPE dt ON p.DATTYID = dt.DATTYID) ON isp.PARID = p.PARID LEFT OUTER JOIN ADDDATA ad ON isp.PARID = ad.PARID LEFT OUTER JOIN ADDTYPE adt ON ad.ADDTYID = adt.ADDTYID LEFT OUTER JOIN BLOCK bl ON isp.BLOID = bl.BLOID LEFT OUTER JOIN DYNAMICBLOCK db ON isp.DYNID = db.DYNID LEFT OUTER JOIN CRC c ON isp.CRCID = c.CRCID LEFT OUTER JOIN MAPPED mp ON isp.MAPID = mp.MAPID WHERE (isp.SERUNFRID = x_x) and ((isp.PARID is not null) OR (isp.CRCID is not null) OR (isp.MAPID is not null)) ";
	sql_map[st_data_stream_receive_info] = xx;


	xx.name = "GPC";
	xx.sql = "SELECT DISCVAL.*,I_PARDIS.PARID,I_PARDIS.OUTOFRANGE  FROM I_SCRPAR, PARAM, I_PARDIS, DISCVAL  WHERE I_SCRPAR.SCRID =x_x  AND I_SCRPAR.PARID=x_x AND I_SCRPAR.PARID=PARAM.PARID  AND PARAM.PARID=I_PARDIS.PARID  AND I_PARDIS.DISCID=DISCVAL.DISCID ";
	sql_map[st_data_stream_state_disc] = xx;


	xx.name = "DSD";
	xx.sql = "SELECT su.SERUNID, p.PARSNAME FROM I_ECUSER ie INNER JOIN SERVICE s ON ie.SERID = s.SERID INNER JOIN SERVUNIT su ON s.SERID = su.SERID INNER JOIN SERVUNITFRAME sf ON su.SERUNID = sf.SERUNID INNER JOIN I_SERPAR isp ON sf.SERUNFRID = isp.SERUNFRID INNER JOIN PARAM p ON isp.PARID = p.PARID WHERE ie.ECUVEID=x_x AND sf.SERUNFRTYID=1 AND s.SERSNAME IN ('RDBLID', 'RECUID', 'MODE01', 'MODEA4', 'RDBI', 'SRBLID') AND p.PARSNAME IN ('x_x') AND (isp.PARID IS NOT NULL OR isp.CRCID IS NOT NULL OR isp.MAPID IS NOT NULL)";
	sql_map[st_data_stream_line_disc] = xx;

	xx.name = "DSD";
	xx.sql = "select * from CONVFORM co, CONFORMTYPE cft  where co.CONTYID = cft.CONTYID and co.parid=x_x ";
	sql_map[st_data_stream_algorithm] = xx;

	xx.name = "DSD";
	xx.sql = "select * from CONVFORM co, CONFORMTYPE cft  where co.CONTYID = cft.CONTYID";
	sql_map[st_gloabl_conver] = xx;
}
string c_sql::replace_parm(string& ori_val, vector<string>& vec_val)
{
	unsigned int i = 0;
	std::string::size_type   pos(0);
	string old_value = "x_x";
	for (i = 0;i < vec_val.size();i++)
	{
		if ((pos = ori_val.find(old_value)) != std::string::npos)
		{
			ori_val.replace(pos, old_value.length(), vec_val[i]);
		}
	}
	return   ori_val;
}