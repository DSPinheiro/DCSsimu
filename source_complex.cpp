/*
 * Source_complex.cpp
 *
 *  Created on: Nov 1, 2020
 *      Author: danip
 */

#include "source_complex.hh"


using namespace std;


extern Geolengthelements Geolengthelements;
extern GeoParameters GeoParameters;
extern GeoParapathlengths GeoParapathlengths;
extern plotparameters plotparameters;
extern Graph_options Graph_options;
extern temperature_parameters temperature_parameters;
extern numberrays numberrays;
extern UserSettings UserSettings;
extern fullenergyspectrum fullenergyspectrum;
extern CurveVerticalTilt CurveVerticalTilt;
extern Curved_Crystal Curved_Crystal;
extern physical_parameters physical_parameters;
extern polarization_parameters polarization_parameters;

extern pick picks[5];
extern vector<energy_gen> Energy_spec;

extern double teta_crys1, Mini_angl, Maxi_angl, d_lat;

extern bool export_prof;

extern ofstream hist_para, hist_anti, gener_out;

extern double peak_posi_para, peak_posi_anti, max_plot[2][2], min_plot[2][2];

double counts_sour, counts_C1, counts_C2_para, counts_C2_anti, counts_detc_para, counts_detc_anti, counts_C2_para_t, counts_detc_para_t, counts_C2_anti_t, counts_detc_anti_t;

double max_hist[6], amplitu_con_para, amplitu_con_anti, middle_energy;

double hist_image_plate_source[n_his_ima][n_his_ima] = {0}, hist_image_plate_crystal1[n_his_ima][n_his_ima] = {0}, hist_image_plate_crystal2_para[n_his_ima][n_his_ima] = {0};
double hist_image_plate_crystal2_anti[n_his_ima][n_his_ima] = {0}, hist_image_plate_detc_para[n_his_ima][n_his_ima] = {0}, hist_image_plate_detc_anti[n_his_ima][n_his_ima] = {0};

double hist_G_function[n_his_g][n_his_g];


void Source_complex::run_Source(SimulationMain *w){

    double max_valu_para, max_valu_anti, S_aper_R_2, S_aper_var_2, S_aper_sqr, S_sour_2, z_sour_2, y_sour_2, zdetc_2_max, zdetc_2_min, ydetc_2_max, ydetc_2_min, tetaref, tetabrag_ref;
    double cos_e, cos2_e, sin_e, tan_e, n1x, n1y, n1z, Dis_total, teta_max, teta_min, squa_tilt1, squa_tilt2, twtilt_C1, delrot_min, delrot_max, delrot_inc, cos_tilt_C1, sin_tilt_C1;
    double delrot, d_lat1_para, d_lat2_para, d_lat1_anti, d_lat2_anti, tw_d1_para, tw_d2_para, tw_d1_anti, tw_d2_anti, a_lamds_uni, b_lamds_uni, del_teta, cosdel, fi_min, auxBragg;
    double fi_max, del_fi, abs_fi_max, cosdel_othe, cosdel_teta, cosdel_teta_othe, n2x_para, n2y_para, n2x_anti, n2y_anti, n2z_para, n2z_anti, p, tetadir, tilt_C1_temp, n1x_temp, n1y_temp;
    double fidir, z, y, r, tetap, sin_fi, cos_fi, sin_tetadir, cos_tetadir, rx, ry, rz, angle, lamda, tetabra1, tetabra2, sin_teref_tedi, teta_max_L, teta_min_L, del_teta_L, inc_tem;
    double toint_para_nor, toint_anti_nor, angle_para, angle_anti, r2x, r2y, r2z, r3x, r3y, r3z, pha_temp[4], fi_max_L, fi_min_L, del_fi_L, tetartab;
    double cos_tetartab, sin_tetartab, cos_difteC1_Ta, sin_difteC1_Ta, cos_tetartabdete_para, sin_tetartabdete_para, cos_tetartabdete_anti, sin_tetartabdete_anti, z_max_C2, z_min_C2;
    double LT_aper_Db, dist_T_Cr1_Db, dist_Cr1_Cr2_Db, dist_Cr2_det_Db, y_min_aper, y_max_aper, z_min_aper, z_max_aper, y_min_C1, y_max_C1, y_min_C2, y_max_C2, z_max_C1, z_min_C1;
    double tilt_C2_para_temp, tilt_C2_anti_temp, n2x_para_temp, n2y_para_temp, n2x_anti_temp;
    double n2y_anti_temp, cos_difteC2_det_para, sin_difteC2_det_para, tan_tetadir, cos_tetadirCry1, sin_fidir, cos_fidir, tan_fidir, cos_fidirtilt, sin_tetap, cos_tetap, var_temp;
    double y_pro_C1, z_pro_C1, sin_tetadirCry1, Costeta_CHC, Sinteta_CHC, rx_rot, ry_rot, cos_fidirtilt2_para, cos_fidirtilt2_anti, corr_dis, sin_tetatab_del_dir, rx_rot_sec, ry_rot_sec;
    double tetadir_det, tan_tetadir_det, cos_tetadir_det, fidir_det, tan_fidir_det, cos_fidir_det, corr_dis_d_pa, y_pro_C1_d_pa, cos_tetap_det, sin_tetap_det, z_det, y_det, r_det;

    vector<int> int_time_out;
    int int_time_out_begg, int_time_mili_out_begg, toint_para, toint_anti, total_para, total_anti, bin_tem = 1, bin_fas = 1;

    int numbins, max_para, I, n_rota, toint_para_total[plotparameters.nubins] = {0}, toint_anti_total[plotparameters.nubins] = {0};

    bool make_G_function, sec_crystal_Parallel_reach, sec_crystal_Antiparallel_reach, reach, first_crystal_reach, cond_rotation, poliP;

    sec_crystal_Parallel_reach = false;
    sec_crystal_Antiparallel_reach = false;
    max_valu_para = 0;
    max_valu_anti = 0;


    make_G_function = false;


    pha_temp[0] = ((double)rand() / RAND_MAX) * 2 * M_PI;
    pha_temp[1] = ((double)rand() / RAND_MAX) * 2 * M_PI;
    pha_temp[2] = ((double)rand() / RAND_MAX) * 2 * M_PI;
    pha_temp[3] = ((double)rand() / RAND_MAX) * 2 * M_PI;


    d_lat1_para = Latice_temp(d_lat, temperature_parameters.T_crystal_1_para);
    d_lat1_anti = Latice_temp(d_lat, temperature_parameters.T_crystal_1_anti);
    d_lat2_para = Latice_temp(d_lat, temperature_parameters.T_crystal_2_para);
    d_lat2_anti = Latice_temp(d_lat, temperature_parameters.T_crystal_2_anti);


    tw_d1_para = 2 * d_lat1_para;
    tw_d1_anti = 2 * d_lat1_anti;
    tw_d2_para = 2 * d_lat2_para;
    tw_d2_anti = 2 * d_lat2_anti;


    S_aper_R_2 = Geolengthelements.S_aper / 2;
    S_aper_var_2 = Geolengthelements.S_aper_var / 2;
    S_aper_sqr = pow(S_aper_R_2, 2);
    S_sour_2 = Geolengthelements.S_sour / 2;

    z_sour_2 = Geolengthelements.z_sour / 2;
    y_sour_2 = Geolengthelements.y_sour / 2;


    zdetc_2_max = Geolengthelements.zdetc / 2 + Geolengthelements.shift_det_ver;
    zdetc_2_min = -Geolengthelements.zdetc / 2 + Geolengthelements.shift_det_ver;
    ydetc_2_max = Geolengthelements.ydetc / 2 + Geolengthelements.shift_det_ver;
    ydetc_2_min = -Geolengthelements.ydetc / 2 + Geolengthelements.shift_det_ver;


    delrot_min = Mini_angl * M_PI / 180;
    delrot_max = Maxi_angl * M_PI / 180;
    delrot_inc = (delrot_max - delrot_min) / (double)plotparameters.nubins;


    delrot = delrot_max;


    numbins = 0;


    tetaref = M_PI / 2 - teta_crys1;

    cos_e = cos(tetaref);
    sin_e = sin(tetaref);


    if(CurveVerticalTilt.make_CurveTilt)
        tilt_C1_temp = ObtainVertical::ObtainVert(1, 0);
    else
        tilt_C1_temp = GeoParameters.tilt_C1;


    cos_tilt_C1 = cos(tilt_C1_temp);
    sin_tilt_C1 = sin(tilt_C1_temp);

    n1x = -cos_tilt_C1 * sin_e;
    n1y = cos_tilt_C1 * cos_e;
    n1z = sin_tilt_C1;


    tetabrag_ref = asin(-n1x);

    if(Curved_Crystal.Curve_crystall){
        n1x_temp = n1x;
        n1y_temp = n1y;
    }


    if(GeoParapathlengths.type_source == "UC"){
        teta_max_L = atan((Geolengthelements.S_aper - Geolengthelements.S_shi_hor_B + Geolengthelements.S_shi_hor_A) / GeoParapathlengths.LT_aper);
        teta_min_L = - atan((Geolengthelements.S_aper + Geolengthelements.S_shi_hor_B - Geolengthelements.S_shi_hor_A) / GeoParapathlengths.LT_aper);
    }else{
        teta_max_L = atan(min((Geolengthelements.y_aper / 2 + y_sour_2 + Geolengthelements.S_shi_hor_B - Geolengthelements.S_shi_hor_A) / GeoParapathlengths.LT_aper, (Geolengthelements.y_first_crys / 2 + y_sour_2 - Geolengthelements.S_shi_hor_A) / (GeoParapathlengths.LT_aper + GeoParapathlengths.dist_T_Cr1)));
        teta_min_L = - atan(min((Geolengthelements.y_aper / 2 + y_sour_2 - Geolengthelements.S_shi_hor_B + Geolengthelements.S_shi_hor_A) / GeoParapathlengths.LT_aper, (Geolengthelements.y_first_crys / 2 + y_sour_2 + Geolengthelements.S_shi_hor_A) / (GeoParapathlengths.LT_aper + GeoParapathlengths.dist_T_Cr1)));
    }


    if(fullenergyspectrum.make_more_lines == 0){
        auxBragg = asin(picks[1].lamda / tw_d1_para);
        teta_max_L = min(M_PI / 2 + GeoParameters.teta_table + GeoParameters.Exp_crys1 * M_PI / 180 - auxBragg + limitReflec, teta_max_L);
        teta_min_L = max(M_PI / 2 + GeoParameters.teta_table + GeoParameters.Exp_crys1 * M_PI / 180 - auxBragg - limitReflec, teta_min_L);
    }


    del_teta_L = teta_max_L - teta_min_L;


    if(GeoParapathlengths.type_source == "UC"){
        fi_max_L = atan((Geolengthelements.S_aper - Geolengthelements.S_shi_ver_B + Geolengthelements.S_shi_ver_A) / GeoParapathlengths.LT_aper);
        fi_min_L = - atan((Geolengthelements.S_aper + Geolengthelements.S_shi_ver_B - Geolengthelements.S_shi_ver_A) / GeoParapathlengths.LT_aper);
    }else{
        fi_max_L = atan(min((Geolengthelements.z_aper / 2 + z_sour_2 + Geolengthelements.S_shi_ver_B - Geolengthelements.S_shi_ver_A) / GeoParapathlengths.LT_aper, (Geolengthelements.z_first_crys / 2 + z_sour_2 - Geolengthelements.S_shi_ver_A) / (GeoParapathlengths.LT_aper + GeoParapathlengths.dist_T_Cr1)));
        fi_min_L = - atan(min((Geolengthelements.z_aper / 2 + z_sour_2 - Geolengthelements.S_shi_ver_B + Geolengthelements.S_shi_ver_A) / GeoParapathlengths.LT_aper, (Geolengthelements.z_first_crys / 2 + z_sour_2 + Geolengthelements.S_shi_ver_A) / (GeoParapathlengths.LT_aper + GeoParapathlengths.dist_T_Cr1)));
    }

    del_fi_L = fi_max_L - fi_min_L;

    tetartab = GeoParameters.teta_table / 2;
    cos_tetartab = cos(GeoParameters.teta_table);
    sin_tetartab = sin(GeoParameters.teta_table);
    cos_difteC1_Ta = cos(GeoParameters.teta_table - tetaref);
    sin_difteC1_Ta = sin(GeoParameters.teta_table - tetaref);

    cos_tetartabdete_para = cos(GeoParameters.teta_table + GeoParameters.teta_detec_para);
    sin_tetartabdete_para = sin(GeoParameters.teta_table + GeoParameters.teta_detec_para);

    cos_tetartabdete_anti = cos(GeoParameters.teta_table + GeoParameters.teta_detec_anti);
    sin_tetartabdete_anti = sin(GeoParameters.teta_table + GeoParameters.teta_detec_anti);

    max_hist[0] = 0;
    max_hist[1] = 0;


    if(tw_d1_anti < tw_d2_anti){
        a_lamds_uni = tw_d1_anti * sin(tetaref + delrot_min - 0.2);
        b_lamds_uni = tw_d2_anti * sin(tetaref + delrot_max + 0.2);
    }else{
        a_lamds_uni = tw_d2_anti * sin(tetaref + delrot_min - 0.2);
        b_lamds_uni = tw_d1_anti * sin(tetaref + delrot_max + 0.2);
    }

    b_lamds_uni = b_lamds_uni - a_lamds_uni;


    counts_sour = 0;
    counts_C1 = 0;
    counts_C2_para = 0;
    counts_C2_anti = 0;
    counts_detc_para = 0;
    counts_detc_anti = 0;
    counts_C2_para_t = 0;
    counts_detc_para_t = 0;
    counts_C2_anti_t = 0;
    counts_detc_anti_t = 0;

    min_plot[0][0] = Mini_angl + teta_crys1 * 180 / M_PI;
    min_plot[1][0] = Mini_angl - teta_crys1 * 180 / M_PI;
    min_plot[0][0] = Maxi_angl + teta_crys1 * 180 / M_PI;
    min_plot[1][0] = Maxi_angl - teta_crys1 * 180 / M_PI;


    LT_aper_Db = (double)GeoParapathlengths.LT_aper;
    dist_T_Cr1_Db = (double)GeoParapathlengths.dist_T_Cr1;
    dist_Cr1_Cr2_Db = (double)GeoParapathlengths.dist_Cr1_Cr2;
    dist_Cr2_det_Db = (double)GeoParapathlengths.dist_Cr2_Det;


    y_min_aper = Geolengthelements.S_shi_hor_A - Geolengthelements.y_aper / 2;
    y_max_aper = Geolengthelements.S_shi_hor_A + Geolengthelements.y_aper / 2;
    z_min_aper = Geolengthelements.S_shi_ver_A - Geolengthelements.z_aper / 2;
    z_max_aper = Geolengthelements.S_shi_ver_A + Geolengthelements.z_aper / 2;

    y_min_C1 = - Geolengthelements.y_first_crys / 2;
    y_max_C1 = Geolengthelements.y_first_crys / 2;
    y_min_C2 = y_min_C1;
    y_max_C2 = y_max_C1;


    if(UserSettings.center_Mask){
        z_max_C1 = 0.6;
        z_min_C1 = - 0.6;

        y_max_C1 = Geolengthelements.y_first_crys / 2 - 0.2;
        y_min_C1 = - Geolengthelements.y_first_crys / 2 + 0.2;
    }else{
        if(UserSettings.mask_C1 == 0){
            z_max_C1 = Geolengthelements.z_first_crys / 2;
            z_min_C1 = - Geolengthelements.z_first_crys / 2;
        }else if(UserSettings.mask_C1 == 1){
            z_max_C1 = Geolengthelements.z_first_crys / 2 - 0.2;
            z_min_C1 = 0;
            y_max_C1 = Geolengthelements.y_first_crys / 2 - 0.2;
            y_min_C1 = - Geolengthelements.y_first_crys / 2 + 0.2;
        }else if(UserSettings.mask_C1 == 2){
            z_max_C1 = 0;
            z_min_C1 = - Geolengthelements.z_first_crys / 2 + 0.2;
            y_max_C1 = Geolengthelements.y_first_crys / 2 - 0.2;
            y_min_C1 = - Geolengthelements.y_first_crys / 2 + 0.2;
        }
    }


    if(UserSettings.mask_C2 == 0){
        z_max_C2 = Geolengthelements.z_first_crys / 2;
        z_min_C2 = - Geolengthelements.z_first_crys / 2;
    }else if(UserSettings.mask_C2 == 1){
        z_max_C2 = Geolengthelements.z_first_crys / 2 - 0.2;
        z_min_C2 = 0;
        y_max_C2 = Geolengthelements.y_first_crys / 2 - 0.2;
        y_min_C2 = - Geolengthelements.y_first_crys / 2 + 0.2;
    }else if(UserSettings.mask_C2 == 2){
        z_max_C2 = 0;
        z_min_C2 = - Geolengthelements.z_first_crys / 2 + 0.2;
        y_max_C2 = Geolengthelements.y_first_crys / 2 - 0.2;
        y_min_C2 = - Geolengthelements.y_first_crys / 2 + 0.2;
    }

    if(Graph_options.make_graph_profile and (not Graph_options.make_image_plates)){
        //implement gui plotting
    }

    if(Graph_options.make_image_plates){
        Make_graph_imageplate::initPlates();
    }

    int_time_out.push_back(0);
    int_time_out.push_back(0);
    int_time_out = Obtain_time::simuTime(0, 0, int_time_out[0], int_time_out[1], w);
    int_time_out_begg = int_time_out[0];
    int_time_mili_out_begg = int_time_out[1];


    n_rota = 1;

    while(n_rota <= numberrays.number_rotati){
        if(n_rota % 2 == 1)
            numbins = 0;
        else
            numbins++;


        cond_rotation = true;

        while(cond_rotation){
            if(n_rota % 2 == 1)
                numbins++;
            else
                numbins--;


            toint_para = 0;
            toint_anti = 0;


            if(temperature_parameters.mk_temp_bin){
                inc_tem = Get_new_temp_for_bins::getNewTemp(bin_tem, bin_fas, pha_temp[2]);
                cout << "inc_te: " << inc_tem << endl;

                d_lat2_para = Latice_temp(d_lat, temperature_parameters.T_crystal_2_para + inc_tem);

                inc_tem = Get_new_temp_for_bins::getNewTemp(bin_tem, bin_fas, pha_temp[3]);
                d_lat2_anti = Latice_temp(d_lat, temperature_parameters.T_crystal_2_anti + inc_tem);

                bin_tem++;

                tw_d1_para = 2 * d_lat1_para;
                tw_d1_anti = 2 * d_lat1_anti;
                tw_d2_para = 2 * d_lat2_para;
                tw_d2_anti = 2 * d_lat2_anti;

                cout << "tw_1: " << d_lat1_para << endl;
                cout << "tw_2: " << d_lat2_para << endl;
            }

            angle_para = delrot * 180 / M_PI + teta_crys1 * 180 / M_PI;
            angle_anti = delrot * 180 / M_PI - teta_crys1 * 180 / M_PI;


            if(CurveVerticalTilt.make_CurveTilt){
                tilt_C2_para_temp = ObtainVertical::ObtainVert(2, angle_para);
                tilt_C2_anti_temp = ObtainVertical::ObtainVert(2, angle_anti);
            }else{
                tilt_C2_para_temp = GeoParameters.tilt_C2;
                tilt_C2_anti_temp = GeoParameters.tilt_C2;
            }


            n2x_para = cos(tilt_C2_para_temp) * sin(GeoParameters.teta_table - tetaref + delrot);
            n2y_para = - cos(tilt_C2_para_temp) * cos(GeoParameters.teta_table - tetaref + delrot);
            n2z_para = sin(tilt_C2_para_temp);
            n2x_anti = - cos(tilt_C2_anti_temp) * sin(GeoParameters.teta_table + tetaref + delrot);
            n2y_anti = cos(tilt_C2_anti_temp) * cos(GeoParameters.teta_table + tetaref + delrot);
            n2z_anti = sin(tilt_C2_anti_temp);

            if(Curved_Crystal.Curve_crystall){
                n2x_para_temp = n2x_para;
                n2y_para_temp = n2y_para;
                n2x_anti_temp = n2x_anti;
                n2y_anti_temp = n2y_anti;
            }


            cos_difteC2_det_para = cos(tetaref - delrot + GeoParameters.teta_table + GeoParameters.teta_detec_para);
            sin_difteC2_det_para = sin(tetaref - delrot + GeoParameters.teta_table + GeoParameters.teta_detec_para);

            I = 1;

            while(I <= numberrays.nbeams){

                if(UserSettings.Make_Horizontal){
                    p = del_teta_L * ((double)rand() / RAND_MAX) + teta_min_L;
                    tetadir = p;
                }else
                    tetadir = 0;


                sin_tetadir = sin(tetadir);
                cos_tetadir = cos(tetadir);
                tan_tetadir = sin_tetadir / cos_tetadir;
                cos_tetadirCry1 = cos(tetadir + GeoParameters.teta_table);


                if(UserSettings.Make_Vertical){
                    p = del_fi_L * ((double)rand() / RAND_MAX) + fi_min_L;
                    fidir = p;
                }else
                    fidir = GeoParameters.xsi;


                sin_fidir = sin(fidir);
                cos_fidir = cos(fidir);
                tan_fidir = sin_fidir / cos_fidir;
                cos_fidirtilt = cos(fidir + tilt_C1_temp);


                if(GeoParapathlengths.type_source == "P"){
                    z = Geolengthelements.S_shi_ver_B;
                    y = Geolengthelements.S_shi_hor_B;
                    r = sqrt(pow(z, 2) + pow(y, 2));

                    if(Geolengthelements.S_shi_ver_B == 0 and Geolengthelements.S_shi_hor_B == 0){
                        sin_tetap = 0;
                        cos_tetap = 1;
                    }else if(Geolengthelements.S_shi_hor_B == 0){
                        sin_tetap = 1;
                        cos_tetap = 0;
                    }else{
                        var_temp = z / y;
                        if(y > 0){
                            sin_tetap = var_temp / sqrt(1 + pow(var_temp, 2));
                            cos_tetap = 1 / sqrt(1 + pow(var_temp, 2));
                        }else{
                            sin_tetap = - var_temp / sqrt(1 + pow(var_temp, 2));
                            cos_tetap = - 1 / sqrt(1 + pow(var_temp, 2));
                        }
                    }

                }else if(GeoParapathlengths.type_source == "UC"){
                    r = S_sour_2 + 1;
                    while(r > S_sour_2){
                        z = ((double)rand() / RAND_MAX) * Geolengthelements.S_sour - S_sour_2;
                        y = ((double)rand() / RAND_MAX) * Geolengthelements.S_sour - S_sour_2;
                        r = sqrt(pow(z, 2) + pow(y, 2));
                    }


                    z += Geolengthelements.S_shi_ver_B;
                    y += Geolengthelements.S_shi_hor_B;

                    if(y != 0)
                        var_temp = z / y;
                    else
                        var_temp = 0;

                    r = sqrt(pow(z, 2) + pow(y, 2));

                    if(y > 0){
                        sin_tetap = var_temp / sqrt(1 + pow(var_temp, 2));
                        cos_tetap = 1 / sqrt(1 + pow(var_temp, 2));
                    }else{
                        sin_tetap = - var_temp / sqrt(1 + pow(var_temp, 2));
                        cos_tetap = - 1 / sqrt(1 + pow(var_temp, 2));
                    }
                }else if(GeoParapathlengths.type_source == "G"){
                    p = 2 * M_PI * ((double)rand() / RAND_MAX);
                    tetap = p;
                    sin_tetap = sin(tetap);
                    cos_tetap = cos(tetap);
                    r = S_aper_R_2 + 1;

                    while(r > S_aper_R_2)
                        r = Box(S_aper_var_2, 0);

                    if(not (Geolengthelements.S_shi_ver_B == 0 and Geolengthelements.S_shi_hor_B == 0)){
                        z = r * sin_tetap +  Geolengthelements.S_shi_ver_B;
                        y = r * cos_tetap +  Geolengthelements.S_shi_hor_B;
                        var_temp = z / y;
                        r = sqrt(pow(z, 2) + pow(y, 2));

                        if(y > 0){
                            sin_tetap = var_temp / sqrt(1 + pow(var_temp, 2));
                            cos_tetap = 1 / sqrt(1 + pow(var_temp, 2));
                        }else{
                            sin_tetap = - var_temp / sqrt(1 + pow(var_temp, 2));
                            cos_tetap = - 1 / sqrt(1 + pow(var_temp, 2));
                        }
                    }

                }else if(GeoParapathlengths.type_source == "UR"){
                    z = ((double)rand() / RAND_MAX) * Geolengthelements.z_sour - z_sour_2;
                    y = ((double)rand() / RAND_MAX) * Geolengthelements.y_sour - y_sour_2;

                    z += Geolengthelements.S_shi_ver_B;
                    y += Geolengthelements.S_shi_hor_B;
                    var_temp = z / y;
                    r = sqrt(pow(z, 2) + pow(y, 2));

                    if(y > 0){
                        sin_tetap = var_temp / sqrt(1 + pow(var_temp, 2));
                        cos_tetap = 1 / sqrt(1 + pow(var_temp, 2));
                    }else{
                        sin_tetap = - var_temp / sqrt(1 + pow(var_temp, 2));
                        cos_tetap = - 1 / sqrt(1 + pow(var_temp, 2));
                    }
                }else{
                    runtime_error("Bad input on the source type: type_source");
                }

                vector<double> yz = getYZ(r, sin_tetap, cos_tetap, tan_tetadir, tan_fidir, LT_aper_Db);

                y = yz[0];
                z = yz[1];

                var_temp = pow(y - Geolengthelements.S_shi_hor_A, 2) + pow(z - Geolengthelements.S_shi_ver_A, 2);

                if(var_temp < S_aper_sqr){

                    if(Graph_options.make_image_plates){
                        Make_pointcryst::Make(1, y, z);
                    }


                    r = sqrt(pow(y, 2) + pow(z, 2));

                    vector<double> yz = getYZ(r, sin_tetap, cos_tetap, tan_tetadir, tan_fidir, LT_aper_Db);

                    y = yz[0];
                    z = yz[1];


                    y_pro_C1 = y * cos_tetadir / cos_tetadirCry1;
                    z_pro_C1 = z * cos_fidir / cos_fidirtilt;

                    //Deprecated???
                    //if(physical_parameters.make_length_corr){
                    //	sin_tetadirCry1 = sin(tetadir + GeoParameters.teta_table);
                    //	make_length_cor(z_pro_C1, y_pro_C1, sin_fidir, cos_fidir, sin_tetadirCry1, z_pro_C1, y_pro_C1);
                    //}


                    //cout << y_pro_C1 << "\t" << y_max_C1 << "\t" << y_min_C1 << "\t;\t" << z_pro_C1 << "\t" << z_max_C1 << "\t" << z_min_C1 << endl;

                    if(y_pro_C1 < y_max_C1 and y_pro_C1 > y_min_C1 and z_pro_C1 < z_max_C1 and z_pro_C1 > z_min_C1){
                        if(not Graph_options.make_imageC1_After_refle){
                            if(Graph_options.make_image_plates){
                                Make_pointcryst::Make(2, y_pro_C1, z_pro_C1);
                            }
                        }

                        rx = cos_fidir * cos_tetadir;
                        ry = cos_fidir * sin_tetadir;
                        rz = sin_fidir;


                        if(Curved_Crystal.Curve_crystall){
                            vector<double> corrRes = Obtain_Curved_Hor_Corr::horCorr(y_pro_C1, y_max_C1, z_pro_C1, z_max_C1, true);

                            Costeta_CHC = corrRes[0];
                            Sinteta_CHC = corrRes[1];

                            n1x = n1x_temp * Costeta_CHC + n1y_temp * Sinteta_CHC;
                            n1y = n1y_temp * Costeta_CHC - n1x_temp * Sinteta_CHC;
                        }

                        vector<double> angleRes = getFullAngle(rx, ry, rz, n1x, n1y, n1z);
                        angle = angleRes[0];
                        r2x = angleRes[1];
                        r2y = angleRes[2];
                        r2z = angleRes[3];


                        lamda = IntensitySource::getEnergy(a_lamds_uni, b_lamds_uni, tw_d1_para);

                        tetabra1 = asin(lamda / tw_d1_para);

                        //cout << angle << "\t" << tetabra1 << endl;

                        if(((double)rand() / RAND_MAX) < polarization_parameters.relationP_S)
                            poliP = true;
                        else
                            poliP = false;


                        first_crystal_reach = Gaussi_rockin::getReflection(angle, tetabra1, lamda, false, poliP);


                        if(first_crystal_reach){


                            if(Graph_options.make_imageC1_After_refle){
                                if(Graph_options.make_image_plates){
                                    Make_pointcryst::Make(2, y_pro_C1, z_pro_C1);
                                }
                            }


                            rx_rot = cos_tetartab * r2x + sin_tetartab * r2y;
                            ry_rot = -sin_tetartab * r2x + cos_tetartab * r2y;

                            //cout << ry_rot << "\t" << -sin_tetartab << "\t" << r2x << "\t" << cos_tetartab << "\t" << r2y << endl;

                            var_temp = ry_rot / rx_rot;

                            tetadir = atan(var_temp);
                            tan_tetadir = var_temp;
                            cos_tetadir = cos(tetadir);


                            var_temp = r2z / sqrt(pow(rx_rot, 2) + pow(ry_rot, 2));
                            fidir = atan(var_temp);
                            tan_fidir = var_temp;
                            cos_fidir = cos(fidir);
                            cos_fidirtilt2_para = cos(fidir + tilt_C2_para_temp);
                            cos_fidirtilt2_anti = cos(fidir + tilt_C2_anti_temp);

                            corr_dis = y_pro_C1 * cos_difteC1_Ta;

                            y_pro_C1 = -y_pro_C1 * sin_difteC1_Ta;


                            if(y_pro_C1 == 0){
                                cos_tetap = 0;
                                if(z_pro_C1 > 0)
                                    sin_tetap = 1;
                                else
                                    sin_tetap = -1;
                            }else{
                                var_temp = z_pro_C1 / y_pro_C1;
                                r = sqrt(pow(z_pro_C1, 2) + pow(y_pro_C1, 2));

                                if(y_pro_C1 > 0){
                                    sin_tetap = var_temp / sqrt(1 + pow(var_temp, 2));
                                    cos_tetap = 1 / sqrt(1 + pow(var_temp, 2));
                                }else{
                                    sin_tetap = - var_temp / sqrt(1 + pow(var_temp, 2));
                                    cos_tetap = - 1 / sqrt(1 + pow(var_temp, 2));
                                }

                            }


                            //cout << cos_tetap << "\t" << tan_tetadir << "\t" << dist_Cr1_Cr2_Db << "\t" << corr_dis << endl;

                            vector<double> yz = getYZ(r, sin_tetap, cos_tetap, tan_tetadir, tan_fidir, dist_Cr1_Cr2_Db - corr_dis);

                            y = yz[0];
                            z = yz[1];


                            if(UserSettings.see_para){
                                sin_tetatab_del_dir = sin(tetaref - delrot + tetadir);
                                y_pro_C1 = y * cos_tetadir / sin_tetatab_del_dir;
                                z_pro_C1 = z * cos_fidir / cos_fidirtilt2_para;

                                //Deprecated????
                                //if(make_length_corr)
                                //	make_length_cor(z_pro_C1, y_pro_C1, sin_fidir, cos_fidir, -sin_tetatab_del_dir, z_pro_C1, y_pro_C1);

                                //cout << y_pro_C1 << "\t" << y_max_C2 << "\t" << y_min_C2 << "\t;\t" << z_pro_C1 << "\t" << z_max_C2 << "\t" << z_min_C2 << endl;

                                if(y_pro_C1 < y_max_C2 and y_pro_C1 > y_min_C2 and z_pro_C1 < z_max_C2 and z_pro_C1 > z_min_C2){

                                    if(Graph_options.make_image_plates){
                                        if(not Graph_options.make_imageC2_After_refle){
                                            Make_pointcryst::Make(3, y_pro_C1, z_pro_C1);
                                        }
                                    }


                                    tetabra2 = asin(lamda / tw_d2_para);


                                    if(Curved_Crystal.Curve_crystall){
                                        vector<double> corrRes = Obtain_Curved_Hor_Corr::horCorr(y_pro_C1, y_max_C2, z_pro_C1, z_max_C2, false);

                                        Costeta_CHC = corrRes[0];
                                        Sinteta_CHC = corrRes[1];

                                        n2x_para = n2x_para_temp * Costeta_CHC + n2y_para_temp * Sinteta_CHC;
                                        n2y_para = n2y_para_temp * Costeta_CHC - n2x_para_temp * Sinteta_CHC;
                                    }


                                    vector<double> angleRes = getFullAngle2(r2x, r2y, r2z, n2x_para, n2y_para, n2z_para);
                                    angle = angleRes[0];
                                    r3x = angleRes[1];
                                    r3y = angleRes[2];
                                    r3z = angleRes[3];

                                    sec_crystal_Parallel_reach = Gaussi_rockin::getReflection(angle, tetabra2, lamda, true, poliP);

                                    if(sec_crystal_Parallel_reach){

                                        if(Graph_options.make_image_plates){
                                            if(Graph_options.make_imageC2_After_refle){
                                                Make_pointcryst::Make(3, y_pro_C1, z_pro_C1);
                                            }
                                        }


                                        rx_rot_sec = cos_tetartabdete_para * r3x + sin_tetartabdete_para * r3y;
                                        ry_rot_sec = -sin_tetartabdete_para * r3x + cos_tetartabdete_para * r3y;

                                        var_temp = ry_rot_sec / rx_rot_sec;



                                        tetadir_det = atan(var_temp);
                                        tan_tetadir_det = var_temp;
                                        cos_tetadir_det = cos(tetadir_det);


                                        var_temp = rz / sqrt(pow(rx_rot, 2) + pow(ry_rot, 2));
                                        fidir_det = atan(var_temp);
                                        tan_fidir_det = var_temp;
                                        cos_fidir_det = cos(fidir_det);
                                        cos_fidirtilt2_para = cos(fidir + tilt_C2_para_temp);

                                        corr_dis_d_pa = -y_pro_C1 * cos_difteC2_det_para;
                                        y_pro_C1_d_pa = -y_pro_C1 * sin_difteC2_det_para;


                                        if(y_pro_C1_d_pa == 0){
                                            cos_tetap_det = 0;
                                            if(z_pro_C1 > 0)
                                                sin_tetap_det = 1;
                                            else
                                                sin_tetap_det = -1;
                                        }else{
                                            var_temp = z_pro_C1 / y_pro_C1_d_pa;
                                            r_det = sqrt(pow(z_pro_C1, 2) + pow(y_pro_C1_d_pa, 2));

                                            if(y_pro_C1_d_pa > 0){
                                                sin_tetap_det = var_temp / sqrt(1 + pow(var_temp, 2));
                                                cos_tetap_det = 1 / sqrt(1 + pow(var_temp, 2));
                                            }else{
                                                sin_tetap_det = - var_temp / sqrt(1 + pow(var_temp, 2));
                                                cos_tetap_det = - 1 / sqrt(1 + pow(var_temp, 2));
                                            }
                                        }

                                        vector<double> yz = getYZ(r_det, sin_tetap_det, cos_tetap_det, tan_tetadir_det, tan_fidir_det, dist_Cr2_det_Db - corr_dis_d_pa);

                                        y_det = yz[0];
                                        z_det = yz[1];

                                        if(y_det < ydetc_2_max and y_det > ydetc_2_min and z_det < zdetc_2_max and z_det > zdetc_2_min){

                                            if(Graph_options.make_image_plates){
                                                Make_pointcryst::Make(4, y_det, z_det);
                                            }

                                            toint_para++;

                                        }

                                    }



                                }



                            }


                            if(UserSettings.see_anti){


                                sin_tetatab_del_dir = sin(tetaref + delrot - tetadir);

                                y_pro_C1 = y * cos_tetadir / sin_tetatab_del_dir;
                                z_pro_C1 = z * cos_fidir / cos_fidirtilt2_anti;

                                //Deprecated?????
                                //if(make_length_corr)
                                //	make_length_cor(z_pro_C1, y_pro_C1, sin_fidir, cos_fidir, sin_tetatab_del_dir, z_pro_C1, y_pro_C1);

                                if(y_pro_C1 < y_max_C2 and y_pro_C1 > y_min_C2 and z_pro_C1 < z_max_C2 and z_pro_C1 > z_min_C2){


                                    if(Graph_options.make_imageC2_After_refle){
                                        if(Graph_options.make_image_plates){
                                            Make_pointcryst::Make(5, y_pro_C1, z_pro_C1);
                                        }
                                    }


                                    tetabra2 = asin(lamda / tw_d2_anti);

                                    if(Curved_Crystal.Curve_crystall){
                                        vector<double> corrRes = Obtain_Curved_Hor_Corr::horCorr(-y_pro_C1, y_max_C2, z_pro_C1, z_max_C2, false);

                                        Costeta_CHC = corrRes[0];
                                        Sinteta_CHC = corrRes[1];

                                        n2x_anti = n2x_anti_temp * Costeta_CHC + n2y_anti_temp * Sinteta_CHC;
                                        n2y_anti = n2y_anti_temp * Costeta_CHC - n2x_anti_temp * Sinteta_CHC;
                                    }


                                    vector<double> angleRes = getFullAngle2(r2x, r2y, r2z, n2x_anti, n2y_anti, n2z_anti);
                                    angle = angleRes[0];
                                    r3x = angleRes[1];
                                    r3y = angleRes[2];
                                    r3z = angleRes[3];


                                    sec_crystal_Antiparallel_reach = Gaussi_rockin::getReflection(angle, tetabra2, lamda, true, poliP);

                                    if(sec_crystal_Antiparallel_reach){

                                        if(Graph_options.make_image_plates){
                                            if(Graph_options.make_imageC2_After_refle){
                                                Make_pointcryst::Make(5, y_pro_C1, z_pro_C1);
                                            }
                                        }


                                        rx_rot_sec = cos_tetartabdete_anti * r3x + sin_tetartabdete_anti * r3y;
                                        ry_rot_sec = - sin_tetartabdete_anti * r3x + cos_tetartabdete_anti * r3y;


                                        var_temp = - ry_rot_sec / rx_rot_sec;
                                        tetadir_det = atan(var_temp);
                                        tan_tetadir_det = var_temp;
                                        cos_tetadir_det = cos(tetadir_det);


                                        var_temp = rz / sqrt(pow(rx_rot, 2) + pow(ry_rot, 2));
                                        fidir_det = atan(var_temp);
                                        tan_fidir_det = var_temp;
                                        cos_fidir_det = cos(fidir_det);
                                        cos_fidirtilt2_anti = cos(fidir + tilt_C2_anti_temp);

                                        corr_dis_d_pa = y_pro_C1 * cos_difteC2_det_para;
                                        y_pro_C1_d_pa = y_pro_C1 * sin_difteC2_det_para;


                                        if(y_pro_C1_d_pa == 0){
                                            cos_tetap_det = 0;
                                            if(z_pro_C1 > 0)
                                                sin_tetap_det = 1;
                                            else
                                                sin_tetap_det = -1;
                                        }else{
                                            var_temp = z_pro_C1 / y_pro_C1_d_pa;
                                            r_det = sqrt(pow(z_pro_C1, 2) + pow(y_pro_C1_d_pa, 2));

                                            if(y_pro_C1_d_pa > 0){
                                                sin_tetap_det = var_temp / sqrt(1 + pow(var_temp, 2));
                                                cos_tetap_det = 1 / sqrt(1 + pow(var_temp, 2));
                                            }else{
                                                sin_tetap_det = - var_temp / sqrt(1 + pow(var_temp, 2));
                                                cos_tetap_det = - 1 / sqrt(1 + pow(var_temp, 2));
                                            }
                                        }


                                        vector<double> yz = getYZ(r_det, sin_tetap_det, cos_tetap_det, tan_tetadir_det, tan_fidir_det, dist_Cr2_det_Db - corr_dis_d_pa);

                                        y_det = yz[0];
                                        z_det = yz[1];

                                        if(y_det < ydetc_2_max and y_det > ydetc_2_min and z_det < zdetc_2_max and z_det > zdetc_2_min){

                                            if(Graph_options.make_image_plates){
                                                Make_pointcryst::Make(6, y_det, z_det);
                                            }

                                            toint_anti++;

                                        }

                                    }

                                }

                            }

                        }

                    }

                }

                I++;

            }



            if(Graph_options.make_image_plates){
                emit w->changeStats(counts_sour, counts_C1, counts_C2_para, counts_C2_anti, counts_detc_para, counts_detc_anti, delrot);
            }


            toint_para_total[numbins - 1] += toint_para;
            total_para = toint_para_total[numbins - 1];

            toint_anti_total[numbins - 1] += toint_anti;
            total_anti = toint_anti_total[numbins - 1];


            angle_para = delrot * 180 / M_PI + teta_crys1 * 180 / M_PI;
            angle_anti = delrot * 180 / M_PI - teta_crys1 * 180 / M_PI;


            if(export_prof){
                hist_para << - angle_para << "\t" << toint_para << "\t" << sqrt((double)toint_para) << endl;
                hist_anti << - angle_anti << "\t" << toint_anti << "\t" << sqrt((double)toint_anti) << endl;
            }


            if(Graph_options.make_graph_profile){
                Make_plot_profiles::plotProfiles(angle_para, total_para, angle_anti, total_anti, numbins, w);
            }


            if(n_rota == numberrays.number_rotati){
                if(UserSettings.fitting){
                    Obtain_data_fitting::FitData(numbins, angle_para, total_para, angle_anti, total_anti);

                    if(max_valu_para < total_para){
                        max_valu_para = total_para;
                        peak_posi_para = angle_para;
                        amplitu_con_para = (double)total_para;
                    }


                    if(max_valu_anti < total_anti){
                        max_valu_anti = total_anti;
                        peak_posi_anti = angle_anti;
                        amplitu_con_anti = (double)total_anti;
                    }
                }
            }


            if(numbins % 50 == 0)
                int_time_out = Obtain_time::simuTime(1, (int)((plotparameters.nubins - numbins) / 50), int_time_out[0], int_time_out[1], w);


            counts_C2_para_t += counts_C2_para;
            counts_detc_para_t += counts_detc_para;
            counts_C2_anti_t += counts_C2_anti;
            counts_detc_anti_t += counts_detc_anti;

            counts_C2_para = 0;
            counts_detc_para = 0;
            counts_detc_anti = 0;
            counts_C2_anti = 0;

            fill(&hist_image_plate_crystal2_para[0][0], &hist_image_plate_crystal2_para[0][0] + n_his_ima * n_his_ima, 0);
            fill(&hist_image_plate_crystal2_anti[0][0], &hist_image_plate_crystal2_anti[0][0] + n_his_ima * n_his_ima, 0);
            fill(&hist_image_plate_detc_para[0][0], &hist_image_plate_detc_para[0][0] + n_his_ima * n_his_ima, 0);
            fill(&hist_image_plate_detc_anti[0][0], &hist_image_plate_detc_anti[0][0] + n_his_ima * n_his_ima, 0);

            max_hist[2] = 0;
            max_hist[3] = 0;
            max_hist[4] = 0;
            max_hist[5] = 0;


            if(n_rota % 2 == 1){
                cond_rotation = numbins < plotparameters.nubins;
                delrot = delrot - delrot_inc;
            }else{
                cond_rotation = numbins > 1;
                delrot = delrot + delrot_inc;
            }


        }

        n_rota++;

    }


    if(Graph_options.make_graph_profile){
        //TODO
        //gui stuff to implement
    }


    if(export_prof){
        gener_out << "********************************" << endl;
        gener_out << endl;
        gener_out << " Number of counts un the several geometric elements" << endl;
        gener_out << endl;
        gener_out << legen_counts[0] << "\t" << counts_sour << endl;
        gener_out << legen_counts[1] << "\t" << counts_C1 << endl;
        gener_out << legen_counts[2] << "\t" << counts_C2_para_t << endl;
        gener_out << legen_counts[3] << "\t" << counts_detc_para_t << endl;
        gener_out << legen_counts[4] << "\t" << counts_C2_anti_t << endl;
        gener_out << legen_counts[5] << "\t" << counts_detc_anti_t << endl;
    }

    if(make_G_function and Graph_options.MakeDislin){
        //TODO
        //implement gui plotting
    }



}
