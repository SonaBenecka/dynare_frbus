// Auto-generated FRB/US equations translated from BIMETS MDL.
// Every equation includes a tracking residual/add-factor a_<endogenous variable>.
// Conditional BIMETS IF> blocks are collapsed to max() formulas where possible.

// dmptmax: Financial Sector Monetary policy indicator for both thresholds DMPTMAX equals one when either the unemployment threshold or the inflation threshold is breached.
    dmptmax = max(dmptlur, dmptpi) + a_dmptmax;

// delrff: Federal funds rate, first diff
    delrff = ((rff) - (rff(-1))) + a_delrff;

// dmptlur: Financial Sector Monetary policy indicator for unemployment threshold DMPTLUR equals zero when the unemployment rate is above its threshold (LURTRSH) one when it is below. A logistic function smoothes the transition, imp
    dmptlur = 1/(1+exp(25*(lur-lurtrsh))) + a_dmptlur;

// dmptpi: Financial Sector Monetary policy indicator for inflation threshold DMPTPI equals zero when expected inflation is below its threshold and one when it is above. A logistic function smoothes the transition, improving soluti
    dmptpi = 1/(1+exp(-25*(zpic58-pitrsh))) + a_dmptpi;

// dmptr: Financial Sector Monetary policy indicator for policy rule thresholds DMPTR is initially zero. It remains at that value until either the unemployment threshold or the inflation threshold is breached, after which it equal
    dmptr = max(dmptmax, dmptr(-1)) + a_dmptr;

// dpadj: Wages and Prices Price inflation aggregation adjustment The adjustment factor for non-consumption prices equals the value in the prior quarter plus the price aggregation discrepancy in the prior quarter. Thus, component 
    ((dpadj) - (dpadj(-1))) = (dpgap(-1)) + a_dpadj;

// dpgap: Wages and Prices Price inflation aggregation discrepancy The price inflation aggregation discrepancy (DPGAP) equals the rate of increase of the price for adjusted final sales excluding consumption less the weighted sum o
    dpgap = (pipxnc/400)-( (0.1036498839384806)*((log(phr*pxp) -
        log(phr(-1)*pxp(-1))))+ (0.3410415470273469)*((log(pbfir*pxp) -
        log(pbfir(-1)*pxp(-1))))+ (0.08204590812944115+0.03932014681002743)*((log(pegfr*pxp) -
        log(pegfr(-1)*pxp(-1))))+ (0.05247494124140634+0.05248394123132865)*((log(pegsr*pxp) -
        log(pegsr(-1)*pxp(-1))))+ (+0.328983631621969)*((log(pxr*pxp) - log(pxr(-1)*pxp(-1))))) +
        a_dpgap;

// ebfi: Business Expenditures Business Fixed Investment, cw 2012 Business Fixed Investment is a weighted average of the investment spending of optimizing firms and an ad hoc term in lagged output growth adjusted for the trend gr
    (log(ebfi) -
        log(ebfi(-1))) = (1-0.3835146156808873)*(+ (0.07358150979200171)*(log((qebfi(-1)/ebfi(-1))))+ (0.5222641043182303)*((log((ebfi(-1)))
        - log((ebfi(-2)))))+ (0.3022170849291267)*((log((ebfi(-2))) -
        log((ebfi(-3)))))+ (1)*zebfi)+ (0.3835146156808873)*(((log((xb(-1))) -
        log((xb(-2)))))-(hgpbfir(-1))/400) + a_ebfi;

// ebfin: Business Expenditures Investment in equipment, current dollar
    ebfin = (0.01)*ebfi*pbfir*pxp + a_ebfin;

// ec: Household Expenditures Consumption, cw 2012 dollar (FRB/US definition) The FRB/US concept of total consumer spending is approximated by a fixed-weighted aggregation of its components.
    (log(ec) -
        log(ec(-1))) = (0.7310605131)*log(eco/(eco(-1)))+ (0.157421136)*log(ech/(ech(-1)))+ (0.1115183509)*log((yhpcd+jkcd)/((yhpcd(-1)+jkcd(-1))))
        + a_ec;

// ecd: Household Expenditures Consumer expenditures on durable goods, cw 2012 dollar Household investment in consumer durables is modeled using the polynomial adjustment cost (PAC) framework
    (log(ecd) -
        log(ecd(-1))) = (0.1561499403562654)*log((qecd(-1)/ecd(-1)))+ (-0.05999277989382532)*((log((ecd(-1)))
        - log((ecd(-2)))))+ (1)*zecd+ (11.87185840275941)*zgapc2/400 + a_ecd;

// ech: Household Expenditures Consumer expenditures on housing services, cw 2012 dollar
    ech = (kh(-1))*( ((ech(-1))/(kh(-2)))+ (0.0005280507960180215)+ (-0.005044873655834186)*(ech(-1))/(kh(-2))+ (0.7183359768373699)*(((ech(-1))/(kh(-2)))
        - ((ech(-2))/(kh(-3)))) ) + a_ech;

// ecnia: Household Expenditures Personal consumption expenditures, cw 2012 dollar (NIPA definition) NIPA total consumer spending is approximated by a fixed-weighted aggregation of its components.
    (log(ecnia) -
        log(ecnia(-1))) = (0.735)*log(eco/(eco(-1)))+ (0.1055)*log(ecd/(ecd(-1)))+ (0.1595)*log(ech/(ech(-1)))
        + a_ecnia;

// ecnian: Household Expenditures Personal consumption expenditures, current dollar (NIPA definition)
    ecnian = (0.01)*pcnia*ecnia + a_ecnian;

// eco: Household Expenditures Consumer expenditures on non-durable goods and non-housing services, cw 2012 dollar Consumer spending on nondurables goods and non-housing services is a weighted average of the spending of optimizi
    (log(eco) -
        log(eco(-1))) = (1-0.3075237404993435)*( (0.1584215605793791)*log((qeco(-1)/eco(-1)))+ (0.4118129978512995)*((log((eco(-1)))
        - log((eco(-2)))))+ (1)*zeco)+ (0.3075237404993435)*log((yhl+yht)/(yhl(-1)+yht(-1))) +
        a_eco;

// egfe: Government Federal Government expenditures, CW 2012 dollar
    (log(egfe) -
        log(egfe(-1))) = (-0.0008045327442607825)+ (-0.1204156945139518)*log((egfe(-1)/egfet(-1)))+ (-0.153308131863314)*(log((egfe(-1)))
        - log((egfe(-2))))+ (-0.1035937599294917)*(log((egfe(-2))) -
        log((egfe(-3))))+ (1.50381543495264)*log(egfet/(egfet(-1)))+ (-0.0009835524480447814)*xgap2+ (0.0007256812123007961)*(xgap2(-1))
        + a_egfe;

// egfen: Government Federal Government expenditures, current dollar
    egfen = pegfr*pxp*egfe*0.01 + a_egfen;

// egfet: Government Federal Government expenditures, CW 2012 dollar, Trend
    (log(egfet) -
        log(egfet(-1))) = (-0.3101)+ (-0.1)*log(0.01*(pegfr(-1)*pxp(-1)*egfet(-1)/xgdptn(-1)))+ (1)*(((hggdpt)
        + (hggdpt(-1)) + (hggdpt(-2)) + (hggdpt(-3)))/4)/400 + a_egfet;

// egfl: Government Federal government employee compensation, cw 2012 dollar
    (log(egfl) -
        log(egfl(-1))) = (0.0002417378554669395)+ (-0.0795584254267964)*log((egfl(-1)/egflt(-1)))+ (0.2415347480637823)*(log((egfl(-1)))
        - log((egfl(-2))))+ (-0.06133379374138964)*(log((egfl(-2))) -
        log((egfl(-3))))+ (1.206872311101357)*log(egflt/(egflt(-1)))+ (-0.002507254010777311)*xgap2+ (0.002350674896424264)*(xgap2(-1))
        + a_egfl;

// egfln: Government Federal government employee compensation, current dollar
    egfln = (0.01)*pgfl*egfl + a_egfln;

// egflt: Government Federal government employee compensation, cw 2012 dollar, trend
    (log(egflt) -
        log(egflt(-1))) = (-0.375978)+ (-0.1)*log(0.01*(pgfl(-1)*egflt(-1)/xgdptn(-1)))+ (1)*(((hggdpt)
        + (hggdpt(-1)) + (hggdpt(-2)) + (hggdpt(-3)))/4)/400 + a_egflt;

// egse: Government S&L Government expenditures, CW 2012 dollar
    (log(egse) -
        log(egse(-1))) = (-0.0003205558585065158)+ (-0.1310861864392932)*log((egse(-1)/egset(-1)))+ (0.1315799717205844)*(log((egse(-1)))
        - log((egse(-2))))+ (0.002629649907733779)*(log((egse(-2))) -
        log((egse(-3))))+ (0.9287008343314474)*(log(egset) -
        log(egset(-1)))+ (0.001580665878764003)*xgap2+ (0.001580665878764003)*(xgap2(-1)) + a_egse;

// egsen: Government S&L Government expenditures, current dollar
    egsen = (0.01)*pegsr*pxp*egse + a_egsen;

// egset: Government S&L Government expenditures, CW 2012 dollar, Trend
    (log(egset) -
        log(egset(-1))) = (-0.32)+ (-0.1)*log(0.01*(pegsr(-1)*pxp(-1)*egset(-1)/xgdptn(-1)))+ (1)*(((hggdpt)
        + (hggdpt(-1)) + (hggdpt(-2)) + (hggdpt(-3)))/4)/400 + a_egset;

// egsl: Government S&L government employee compensation, cw 2012 dollar
    (log(egsl) -
        log(egsl(-1))) = (0.0003682770277494773)+ (-0.1114151303946257)*log((egsl(-1)/egslt(-1)))+ (0.1562467669415557)*(log((egsl(-1)))
        - log((egsl(-2))))+ (0.02155815410956755)*(log((egsl(-2))) -
        log((egsl(-3))))+ (0.7449614274122569)*(log(egslt) -
        log(egslt(-1)))+ (-0.001432565493088224)*xgap2+ (0.001765173794440745)*(xgap2(-1)) + a_egsl;

// egsln: Government S&L government employee compensation, current dollar
    egsln = (0.01)*pgsl*egsl + a_egsln;

// egslt: Government S&L government employee compensation, cw 2012 dollar, trend
    (log(egslt) -
        log(egslt(-1))) = (-0.259779)+ (-0.1)*log(0.01*(pgsl(-1)*egslt(-1)/xgdptn(-1)))+ (1)*(((hggdpt)
        + (hggdpt(-1)) + (hggdpt(-2)) + (hggdpt(-3)))/4)/400 + a_egslt;

// eh: Household Expenditures Residential investment expenditures, cw 2012 dollar Household investment in residential structures is modeled using the polynomial adjustment cost (PAC) framework
    (log(eh) -
        log(eh(-1))) = (0.01309931436163227)*log((qeh(-1)/eh(-1)))+ (0.370727139760127)*(log((eh(-1)))
        - log((eh(-2))))+ (0.206060944066708)*(log((eh(-2))) -
        log((eh(-3))))+ (1)*zeh+ (-0.05620618046332654)*(((rme(-1))) -
        ((rme(-2))))+ (0.02793327971442171)*d83*(((rme(-1))) - ((rme(-2)))) + a_eh;

// ehn: Household Expenditures Residential investment expenditures
    ehn = (0.01)*phr*pxp*eh + a_ehn;

// ei: Business Expenditures Change in private inventories, cw 2012 dollar
    ei = (4)*((ki) - (ki(-1))) + a_ei;

// ein: Business Expenditures Change in business inventories, current
    ein = (0.01)*pxp*pkir*ei + a_ein;

// em: Foreign Trade Imports of goods and services, cw 2012 dollar
    log(em) = log((em(-1)))+ (0.5)*(emon/emn+(emon(-1)/emn(-1)))*((log(emo) -
        log(emo(-1))))+ (((empn/emn) + (empn(-1)/emn(-1)))/2)*((log(emp) - log(emp(-1)))) + a_em;

// emn: Foreign Trade Imports of goods and services, current dollar
    emn = emon+empn + a_emn;

// emo: Foreign Trade Imports of goods and services ex. petroleum, cw 2012 dollar The equation for non-petroleum imports has an error-correction format. In the long run, the volume of imports depends on the level of real domesti
    (log(emo) -
        log(emo(-1))) = (0.01620546980581163)+ (-0.1807104996815989)*log((emo(-1)*pmo(-1)/100)/((uemot(-1)*xgdpn(-1))))+ (1.358186927724613)*((xgap2)
        - (xgap2(-1)))/100+ (1.637077186962015)*(((xgap2(-1))) -
        ((xgap2(-2))))/100+ (0.3188307105827468)*log(ddockm)+ (0.4046942138546456)*log(ddockm/(ddockm(-1)))
        + a_emo;

// emon: Foreign Trade Imports of goods and services ex. petroleum
    emon = (0.01)*pmo*emo + a_emon;

// emp: Foreign Trade Petroleum imports, cw 2012 dollar
    log(emp) = log(xgdp)+log(emptrt)+ (0.048026)*((log(pmp/pxb) -
        log(pmp(-1)/pxb(-1))))+ (0.022115)*(xgap2(-1)) + a_emp;

// empn: Foreign Trade Petroleum imports, current dollar
    empn = (0.01)*pmp*emp + a_empn;

// ex: Foreign Trade Exports of goods and services, cw 2012 dollar Exports is modeled in an error-correction format.
    (log(ex) -
        log(ex(-1))) = (0.8398526775741691)+ (-0.1077278728634364)*log((ex(-1)*pxr(-1)*pxp(-1)*fpx(-1))/(fgdp(-1)*fpc(-1)))+ (1.481642245331822)*((fxgap)
        - (fxgap(-1)))/100+ (1.027448537481708)*(((fxgap(-1))) -
        ((fxgap(-2))))/100+ (1.015857050459678)*ddockx + a_ex;

// exn: Foreign Trade Exports of goods and services, current dollar
    exn = (0.01)*pxp*pxr*ex + a_exn;

// fcbn: Foreign Trade US current account balance, current dollar The current account balance is equal to net exports (EXN-EMN) plus net foreign investment income (FYNIN) and a discrepancy term (FCBRN).
    fcbn = exn-emn+fynin+fcbrn + a_fcbn;

// fcbrn: Foreign Trade US current account balance residual, current dollar
    fcbrn = ufcbr*pxb*xbt/100 + a_fcbrn;

// fgdp: Foreign Activity Foreign aggregate GDP (world, bilateral export weights) The level of foreign GDP is determined via the identity that links it to the level of potential foreign output (FGDPT) and the foreign output gap (
    fgdp = fgdpt*exp(fxgap/100) + a_fgdp;

// fgdpt: Foreign Activity Foreign aggregate GDP (world, bilateral export weights), trend In simulations, the level of trend foreign GDP moves in proportion with level of U.S potential GDP in the long run. In the short run, the ra
    (log(fgdpt) - log(fgdpt(-1))) = (-0.458264)+ (-0.1)*log((fgdpt(-1)/xgdpt(-1)))+ (1)*(((hggdpt)
        + (hggdpt(-1)) + (hggdpt(-2)) + (hggdpt(-3)))/4)/400 + a_fgdpt;

// fnicn: Foreign Trade Gross stock of claims of US residents on the rest of the world, current dollar
    ((fnicn) -
        (fnicn(-1))) = (+0.54)*(log(fpc)-(log((fpc(-1)))))*(fnicn(-1))+ (-0.67)*(log(fpx)-(log((fpx(-1)))))*(fnicn(-1))+ rfnict*xgdptn
        + a_fnicn;

// fniln: Foreign Trade Gross stock of liabilities of US residents to the rest of the world, current dollar
    fniln = fnicn-fnin + a_fniln;

// fnin: Foreign Trade Net stock of claims of US residents on the rest of the world, current dollar The change in the net foreign investment position is equal to the the sum of the current account balance expressed at a quarterly
    ((fnin) - (fnin(-1))) = (0.25)*fcbn+ (0.54)*(log(fpc) -
        log(fpc(-1)))*(fnicn(-1))+ (-0.32)*(log(pgdp) -
        log(pgdp(-1)))*(fniln(-1))+ (-0.67)*(log(fpx) - log(fpx(-1)))*(fnicn(-1))+ (0.06)*(log(fpx)
        - log(fpx(-1)))*(fniln(-1))+ +fnirn + a_fnin;

// fnirn: Foreign Trade Net stock of claims of US residents on the rest of the world, residual
    fnirn = ufnir*xgdpn + a_fnirn;

// fpc: Foreign Activity Foreign aggregate consumer price (G39, import/export trade weights)
    fpc = (fpc(-1))*exp(fpic/400) + a_fpc;

// fpi10: Foreign Activity Foreign consumer price inflation (G10) Foreign CPI inflation is a function of a weighted average of past and trend foreign inflation, the foreign output gap, and current and lagged changes in the relativ
    fpi10 = (0.6279749057317539)*((((fpi10(-1))) + ((fpi10(-2))) + ((fpi10(-3))) +
        ((fpi10(-4))))/4)+ (0.3720250942682461)*fpitrg+ (0.3221458278398038)*(fxgap(-1)) + a_fpi10;

// fpi10t: Foreign Activity Foreign consumer price inflation, trend (G10) The trend component of foreign inflation adjusts at 5 percent per quarter to movements in actual foreign inflation.
    fpi10t = (0.95)*(fpi10t(-1))+ (0.05)*fpi10 + a_fpi10t;

// fpic: Foreign Activity Foreign consumer price inflation (G39, bilateral export trade weights) In the long run, foreign consumer price inflation as measured on a G39 basis moves one for one with foreign inflation as measured on
    fpic = (2.027196898567632)+ (0.6788298801616212)*fpi10+ (0.3211701198383788)*(fpic(-1)) + a_fpic;

// fpx: Foreign Activity Nominal exchange rate (G39, import/export trade weights)
    fpx = fpxr*fpc/pcpi + a_fpx;

// fpxr: Foreign Activity Real exchange rate (G39, import/export trade weights) The real exchange rate is determined via an open interest parity condition based on real bond rates, augmented with a term in the ratio of the net st
    log(fpxr) = log(fpxrr)+ (0.048)*(rg10-zpi10f-frl10+fpi10t)+ (0.5)*(fnin/xgdpn) + a_fpxr;

// fpxrr: Foreign Activity Real exchange rate residual The unexplained component of the exchange rate error-corrects to its long-run exogenous trend (FPXRRT).
    (log(fpxrr) -
        log(fpxrr(-1))) = (0.02744058231523356)*log((fpxrrt(-1)/fpxrr(-1)))+ (0.2110896761767876)*((log((fpxrr(-1)))
        - log((fpxrr(-2)))))+ (1-0.2110896761767876)*(log(fpxrrt) - log(fpxrrt(-1))) + a_fpxrr;

// frl10: Foreign Activity Foreign long-term interest rate (G10) The foreign long-term interest rate (FRL10) is modeled using a reduced-form error-correction specification in which the long rate converges to the foreign short-term
    ((frl10) -
        (frl10(-1))) = (0.03753227212139513)+ (-0.07704648128878299)*(frl10(-1)-frs10(-1))+ (0.06550476702266198)*(((frl10(-1)))
        - ((frl10(-2))))+ (0.369056454239088)*((frs10) - (frs10(-1)))+ (0.1245511812496244)*((fxgap)
        - (fxgap(-1))) + a_frl10;

// frs10: Foreign Activity Foreign short-term interest rate (G10) The foreign short-term interest rate (FRS10) is set either according to a version of the Taylor rule (DFMPRR=0), or as the sum of an exogenous real interest rate (R
    frs10 = dfmprr*( (1.0)*(frstar(-1))+ (1.0)*(((fpi10) + (fpi10(-1)) + (fpi10(-2)) +
        (fpi10(-3)))/4)+ (0.5)*((((fpi10) + (fpi10(-1)) + (fpi10(-2)) +
        (fpi10(-3)))/4)/4-fpitrg)+ (1.0)*fxgap)+ (1-dfmprr)*(rfrs10+(((fpi10) + (fpi10(-1)) +
        (fpi10(-2)) + (fpi10(-3)))/4)) + a_frs10;

// frstar: Foreign Activity Equilibrium real short-term interest rate used in foreign Taylor rule The estimate of the foreign equilibrium real short-term interest rate used in the foreign Taylor rule is updated each period by 5 per
    frstar = (0.95)*(frstar(-1))+ (0.05)*(frs10-(((fpi10) + (fpi10(-1)) + (fpi10(-2)) +
        (fpi10(-3)))/4)) + a_frstar;

// ftcin: Foreign Trade Corporate taxes paid to rest of world, current dollar
    ftcin = uftcin*ynicpn + a_ftcin;

// fxgap: Foreign Activity Foreign output gap (world, bilateral export weights) The equation for the foreign output gap is a reduced-form IS curve. The gap depends on lags of the foreign output gap, the real short-term foreign int
    fxgap = (1.290723676327916)*(fxgap(-1))+ (-0.4680091148746248)*(fxgap(-2))+ (-0.05)*(((((frs10(-1))-((((fpi10(-1)))
        + ((fpi10(-2))) + ((fpi10(-3))) + ((fpi10(-4))))/4)) + ((frs10(-2))-((((fpi10(-2))) +
        ((fpi10(-3))) + ((fpi10(-4))) + ((fpi10(-5))))/4)) + ((frs10(-3))-((((fpi10(-3))) +
        ((fpi10(-4))) + ((fpi10(-5))) +
        ((fpi10(-6))))/4)))/3)-frstar)+ (0.03734559019022718)*(xgap2(-1)) + a_fxgap;

// fynicn: Foreign Trade Gross investment income received from the rest of the world, current dollar
    fynicn = (0.01)*rfynic*(fnicn(-1)) + a_fynicn;

// fyniln: Foreign Trade Gross investment income paid to the rest of the world, current dollar
    fyniln = (0.01)*rfynil*(fniln(-1)) + a_fyniln;

// fynin: Foreign Trade Net investment income received from the rest of the world, current dollar
    fynin = fynicn-fyniln + a_fynin;

// gfdbtnp: Government Federal government debt stock held by the public, current dollar
    gfdbtnp = ugfdbtp*((gfdbtnp(-1))-0.25*gfsrpn) + a_gfdbtnp;

// gfdbtn: Government Federal government debt stock held by the public, current dollar
    gfdbtn = ugfdbt*gfdbtnp + a_gfdbtn;

// ugfsrp: Government Federal Government budget surplus, residual
    ugfsrp = (0.065666)*(1-0.947688)+0.947688*(ugfsrp(-1)) + a_ugfsrp;

// uleg: Government Multiplicative factor for government civilian employment
    (log(uleg) - log(uleg(-1))) = (-0.1)*((leg(-1)/lep(-1))-adjlegrt) + a_uleg;

// gfexpn: Government Government payments
    gfexpn = egfln+egfen+gtn+gfintn + a_gfexpn;

// gfintn: Government Federal government net interest payments, current dollar
    gfintn = rgfint*(gfdbtn(-1)) + a_gfintn;

// gfrecn: Government Government receipts and residual
    gfrecn = tpn+tcin+ugfsrp*xgdpn + a_gfrecn;

// gfsrpn: Government Federal government budget surplus, current dollar
    gfsrpn = tpn+tcin-egfln-egfen-gtn-gfintn+ugfsrp*xgdpn + a_gfsrpn;

// gtn: Government Federal government net transfer payments, current dollar
    gtn = (0.01)*pgdp*gtr + a_gtn;

// gtr: Government Government net transfer payments, deflated by PGDP
    gtr = (gtrd+gtrt)*xgdpt + a_gtr;

// gtrd: Government Deviation of ratio of government transfers to GDP from trend ratio
    gtrd = (-0.000176387604876)*xgap2+ (-0.00143470943)*((((xgap2(-1))) + ((xgap2(-2))) +
        ((xgap2(-3))) +
        ((xgap2(-4))))/4)+ (0.862481931486)*( (gtrd(-1))- (-0.000176387604876)*(xgap2(-1))- (-0.00143470943)*((((xgap2(-2)))
        + ((xgap2(-3))) + ((xgap2(-4))) + ((xgap2(-5))))/4) ) + a_gtrd;

// hgemp: Foreign Trade Petroleum imports, cw 2012 dollar, trend growth rate
    hgemp = (0.9)*(hgemp(-1))+ (0.1)*400*log(emp/(emp(-1))) + a_hgemp;

// hggdp: Aggregate Output Identities Growth rate of GDP, cw 2012 dollar (annual rate)
    hggdp = (400)*(log(xgdp) - log(xgdp(-1))) + a_hggdp;

// hggdpt: Aggregate Output Identities Trend growth rate of XGDP, cw 2012 dollar (annual rate)
    hggdpt = hxbt+huxb + a_hggdpt;

// hgpbfir: Aggregate Output Identities Trend growth rate of price of business investment (relative to PXB)
    hgpbfir = (0.975)*(hgpbfir(-1))+0.025*400*log(pbfir*pxp/pxb/(pbfir(-1)*pxp(-1)/pxb(-1))) +
        a_hgpbfir;

// hgpkir: Wages and Prices Trend growth rate of PKIR
    hgpkir = (0.9)*(hgpkir(-1))+ (0.1)*400*log(pkir/(pkir(-1))) + a_hgpkir;

// hgynid: Expectations Growth rate of real after-tax corporate profits
    hgynid = (400)*(log((ynicpn-tcin)*0.5/pxb) - log((ynicpn(-1)-tcin(-1))*0.5/pxb(-1))) + a_hgynid;

// hks: Business Expenditures Growth rate of KS, cw 2012 dollar (compound annual rate)
    hks = (400)*(ykbfin*(log(kbfi) - log(kbfi(-1)))+ ykin*(log(ki) -
        log(ki(-1))))/(ykbfin+ykin)+hksr + a_hks;

// hlept: Labor Market Trend growth rate of LEP (annual rate)
    hlept = (400)*hqlfpr+ (400)*(log(n16) - log(n16(-1))) + a_hlept;

// hlprdt: Labor Market Trend growth rate of output per hour
    hlprdt = hxbt-hlept-hqlww + a_hlprdt;

// hmfpt: Aggregate Output Identities Trend growth rate of multifactor productivity
    hmfpt = (0.055)+ (0.95)*(hmfpt(-1)) + a_hmfpt;

// hqlfpr: Labor Market Drift component of change in QLFPR
    hqlfpr = (0.00)+ (0.95)*(hqlfpr(-1)) + a_hqlfpr;

// hqlww: Labor Market Trend growth rate of workweek
    hqlww = (0.95)*(hqlww(-1))+ (1-0.95)*(-0.3129029344874886) + a_hqlww;

// huqpct: Wages and Prices Drift term in stochastic component of trend ratio of PCNIA to PXP
    huqpct = (0.00)+ (0.95)*(huqpct(-1)) + a_huqpct;

// huxb: Aggregate Output Identities Drift term in UXBT
    huxb = (1-dglprd)*( (-0.01613974358626877)+ (0.95)*(huxb(-1)) ) + a_huxb;

// hxbt: Aggregate Output Identities Trend rate of growth of XB, cw 2012 dollar (annual rate)
    hxbt = (0.725)*(hlept+hqlww+400*(log(lqualt) - log(lqualt(-1))))+ (0.275)*hks+hmfpt+hxbtr + a_hxbt;

// hxbtr: Aggregate Output Identities Residual Factor (Trend rate of growth of XB)
    hxbtr = 0 + a_hxbtr;

// jccan: Nominal Income Consumption of fixed capital, current dollar
    jccan = xgdpn*( (0.053902)*(1-(0.821408))+ (0.821408)*(jccan(-1)/xgdpn(-1))+ (1-0.821408)*(pkbfir(-1)*kbfi(-1))*jrbfi*(pxp(-1)/xgdpn(-1))*0.01 )
        + a_jccan;

// jkcd: Household Expenditures Consumption of fixed capital, consumer durables
    jkcd = jrcd*(kcd(-1)) + a_jkcd;

// kbfi: Business Expenditures Capital stock - BFI, 2012 dollar
    kbfi = (pbfir/pkbfir)*ebfi/4+(1-jrbfi/4)*(kbfi(-1)) + a_kbfi;

// kcd: Household Expenditures Stock of consumer durables, cw 2012 dollar
    kcd = (0.25)*ecd+(1-jrcd/4)*(kcd(-1)) + a_kcd;

// kh: Household Expenditures Stock of residential structures, cw 2012 dollar
    kh = (0.25)*eh+(1-jrh/4)*(kh(-1)) + a_kh;

// ki: Business Expenditures Stock of private inventories, cw 2012 dollar
    (log(ki) -
        log(ki(-1))) = (-0.00120624751419073)+ (0.01469206254903638)*(log(qkir)-log((ki(-1)/xfs(-1))))+ (0.4567399290262596)*((log((ki(-1)))
        - log((ki(-2))))+0.00120624751419073)+ (0.236031927797704)*(log((xfs(-1))) -
        log((xfs(-2))))+ (0.3072281431760363)*(log((xfs(-2))) - log((xfs(-3)))) + a_ki;

// ks: Business Expenditures Capital services, 2012 dollar
    log(ks) = log((ks(-1)))+hks/400 + a_ks;

// leg: Government Government civilian employment ex. gov. enterprise
    log(leg) = log(uleg*(egfl+egsl)/lprdt) + a_leg;

// leh: Labor Market Civilian employment (break adjusted)
    leh = lep+leg+leo + a_leh;

// leo: Labor Market Difference between household and business sector payroll employment, less government employment
    log(leo) = log(qleor*qlf)+ (0.7566675970336151)*log((leo(-1)/(qleor(-1)*qlf(-1))))+ (-0.01642583348237834)*(xgap2(-1))
        + a_leo;

// lep: Labor Market Employment in  business sector (employee and  self-employed)
    lep = lhp/lww + a_lep;

// leppot: Labor Market Potential employment in  business sector
    leppot = (qlf*(1-0.01*lurnat-qleor))/(1+adjlegrt) + a_leppot;

// lf: Labor Market Civilian labor force (break adjusted)
    lf = lfpr*n16 + a_lf;

// lfpr: Labor Market Labor force participation rate
    ((lfpr) -
        (lfpr(-1))) = hqlfpr+ (0.5676074828293328)*(qlfpr(-1)-lfpr(-1))+ (-0.0008751892020969236)*(lur(-1)-lurnat(-1))
        + a_lfpr;

// lhp: Labor Market Aggregate labor hours,  business sector (employee and  self-employed)
    (log(lhp) -
        log(lhp(-1))) = (0.2022897898011354)*log((qlhp(-1)/lhp(-1)))+ (0.2028806748568088)*(log((lhp(-1)))
        - log((lhp(-2))))+ (0.3720641848854363)*zlhp+ (0.6279358151145638)*((log(xbo) -
        log(xbo(-1)))-(hlprdt(-1))/400)+ (-0.127396041937203)*((log((xbo(-1))) -
        log((xbo(-2))))-(hlprdt(-2))/400) + a_lhp;

// lprdt: Labor Market Trend labor productivity
    log(lprdt) = log(xbt/leppot/qlww) + a_lprdt;

// lur: Labor Market Civilian unemployment rate (break adjusted)
    lur = 100*(1-leh/lf) + a_lur;

// lurnat: Labor Market Natural rate of unemployment
    lurnat = (lurnat(-1)) + a_lurnat;

// lww: Labor Market Workweek,  business sector (employee and self-employed)
    (log(lww) -
        log(lww(-1))) = hqlww/400+ (0.195710350653204)*log((qlww(-1)/lww(-1)))+ (0.3184816471961936)*((log(lhp)
        - log(lhp(-1)))-(hlept+hqlww)/400) + a_lww;

// mei: Financial Sector Multiplicative discrepancy for the difference between XGDI and XGDO
    log(mei) = (1.00)*log((mei(-1))) + a_mei;

// mep: Financial Sector Multiplicative discrepancy for the difference between XGDP and XGDO
    log(mep) = (1.00)*log((mep(-1))) + a_mep;

// mfpt: Aggregate Output Identities Multifactor productivity, trend level
    log(mfpt) = (0.0)+log((mfpt(-1)))+hmfpt/400 + a_mfpt;

// pbfir: Wages and Prices Price level of BFI compared to PXP
    log(pbfir) = log((pbfir(-1)))+pipxnc/400+dpadj-(log(pxp) - log(pxp(-1))) + a_pbfir;

// pcdr: Wages and Prices Price index for consumer durables, cw (relative to to PCNIA)
    (log(pcdr) - log(pcdr(-1))) = (-0.003334518696912942)+ (0.5098481943420772)*(log((pcdr(-1))) -
        log((pcdr(-2)))) + a_pcdr;

// pcer: Wages and Prices Price index for personal consumption expenditures on energy (relative to PCXFE)
    (log(pcer) - log(pcer(-1))) = (0.2488609533651416)*(log(pmp/pcxfe) - log(pmp(-1)/pcxfe(-1))) +
        a_pcer;

// pcfr: Wages and Prices Price index for personal consumption expenditures on food (relative to PCXFE)
    (log(pcfr) -
        log(pcfr(-1))) = (-0.1753151166404203)*log((pcfr(-1)/pcfrt(-1)))+ (-5.59454305651988/100000)+ (0.3855084844618753)*(log((pcfr(-1)))
        - log((pcfr(-2))))+ (0.02031018771733943)*(log((pcfr(-2))) -
        log((pcfr(-3))))+ (0.3388841893424357)*(log((pcfr(-3))) -
        log((pcfr(-4))))+ (0.3176985579727371)*(log(pcfrt) - log(pcfrt(-1))) + a_pcfr;

// pchr: Wages and Prices Price index for housing services, cw (relative to to PCNIA)
    (log(pchr) - log(pchr(-1))) = (0.0006138065838894166)+ (0.5980639856669541)*(log((pchr(-1))) -
        log((pchr(-2)))) + a_pchr;

// pcnia: Wages and Prices Price index for personal consumption expenditures, cw (NIPA definition)
    (log(pcnia) - log(pcnia(-1))) = picnia/400 + a_pcnia;

// pcor: Wages and Prices Price index for non-durable goods and non-housing services, cw (relative to to PCNIA
    log(pcor) = log((pcor(-1)))+ (-0.1436)*log(pcdr/(pcdr(-1)))+ (-0.217)*log(pchr/(pchr(-1))) + a_pcor;

// pcpi: Wages and Prices Consumer price index,total
    pcpi = upcpi*pcnia + a_pcpi;

// pcpix: Wages and Prices Consumer price index,excluding food and energy
    pcpix = upcpix*pcxfe + a_pcpix;

// pcxfe: Wages and Prices Price index for personal consumption expendits ex. food and energy, cw (NIPA definit
    (log(pcxfe) - log(pcxfe(-1))) = picxfe/400 + a_pcxfe;

// pegfr: Government Price index for federal government expenditures, CW (relative to PXP)
    (log(pegfr) - log(pegfr(-1))) = pipxnc/400+dpadj-(log(pxp) - log(pxp(-1))) + a_pegfr;

// pegsr: Government Price index for S&L government expenditures, CW (relative to PXP)
    log(pegsr) = log((pegsr(-1)))+pipxnc/400+dpadj-(log(pxp) - log(pxp(-1))) + a_pegsr;

// pgdp: Wages and Prices Price index for GDP, cw
    pgdp = 100*xgdpn/xgdp + a_pgdp;

// pgfl: Wages and Prices Price index for federal government employee compensation, cw
    log(pgfl) = log(upgfl*pl/lprdt) + a_pgfl;

// pgsl: Wages and Prices Price index for S&L government employee compensation, cw
    log(pgsl) = log(upgsl*pl/lprdt) + a_pgsl;

// phouse: Financial Sector Loan Performance House Price Index
    (log(phouse) -
        log(phouse(-1))) = (0.005158779390135185)+ (0.9018869955145716)*(log((phouse(-1))) -
        log((phouse(-2))))+ (-0.01156922658993683)*log((phouse(-1)/(pchr(-1)*pcnia(-1)))) +
        a_phouse;

// phr: Wages and Prices Price index for residential investment, cw (relative to PXP)
    (log(phr) - log(phr(-1))) = (0.0)+pipxnc/400+dpadj-(log(pxp) - log(pxp(-1))) + a_phr;

// pic4: Wages and Prices Four-quarter percent change in PCE prices
    pic4 = (100)*(pcnia/(pcnia(-4))-1) + a_pic4;

// picnia: Wages and Prices Inflation rate, personal consumption expenditures, cw
    picnia = picxfe+ (0.0399)*400*(log(pcer) - log(pcer(-1)))+ (0.0726)*400*(log(pcfr) -
        log(pcfr(-1))) + a_picnia;

// picx4: Wages and Prices Four-quarter percent change core in PCE prices
    picx4 = 100*(pcxfe/(pcxfe(-4))-1) + a_picx4;

// picxfe: Wages and Prices Inflation rate, personal consumption expenditures, ex. food and energy, cw
    picxfe = ( (0.671147437396)*(picxfe(-1))+ (0.98)*zpicxfe+ (1-(0.98))*(1-(0.671147437396))*(ptr(-1))+ (0.00191485982247)*400*log((qpcnia(-1)/pcnia(-1))) )/ (1+(0.671147437396)*(0.98))
        + a_picxfe;

// pieci: Wages and Prices Annualized rate of growth of EI hourly compensation
    pieci = ( (0.25)* (0.684663089102)*((1-(0.98))*(((pieci(-1))) + ((pieci(-2))) +
        ((pieci(-3))))+(pieci(-4)))+ (0.98)*zpieci+ (1-(0.98))*(1-(0.684663089102))*((ptr(-1)+hlprdt(-1)-400*huqpct(-1)))+ (-0.0167232072082)*((lur(-1)-lurnat(-1)))+ (0.000957429911235)*400*log((qpl(-1)/pl(-1))) )/ (1+0.25*(0.684663089102)*(0.98))
        + a_pieci;

// pigdp: Wages and Prices Inflation rate, GDP, cw
    pigdp = 400*(log(pgdp) - log(pgdp(-1))) + a_pigdp;

// pipl: Wages and Prices Rate of growth of PL
    pipl = pieci + a_pipl;

// pipxnc: Wages and Prices Inflation rate, price of adjusted final sales excluding consumption (annual rate)
    pipxnc = picnia+ (-1.99)*400*huqpct+ (0.462801)*(pipxnc(-1)-picnia(-1)+1.99*400*huqpct(-1))+ (0.229745)*(pipxnc(-2)-picnia(-2)+1.99*400*huqpct(-2))+ (-0.284477)*(0.1312355937)*400*(log(fpxr)
        - log(fpxr(-1)))+ (0.025)*400*log((qpxnc(-1)/pxnc(-1))) + a_pipxnc;

// pkbfir: Wages and Prices Ratio of price of BFI stock (KBFI) to PXP
    pkbfir = upkbfir*pbfir + a_pkbfir;

// pl: Wages and Prices Compensation per hour,  business
    log(pl) = log((pl(-1)))+pipl/400 + a_pl;

// pmo: Wages and Prices Price index for imports ex. petroleum, cw
    (log(pmo) -
        log(pmo(-1))) = (0.0)+ (0.3776815983707077)*(log(qpmo/(pmo(-1)))+0.64*log((fpc(-1)/fpx(-1)))+0.36*log((pxb(-1))))+ (0.2343966603329429)*(log(fpc/fpx)
        - log(fpc(-1)/fpx(-1)))+ (1-(0.2343966603329429))*(log(pxb) - log(pxb(-1))) + a_pmo;

// pmp: Wages and Prices Price index for petroleum imports
    pmp = upmp*poil + a_pmp;

// poil: Wages and Prices Price of imported oil ( dollar per barrel)
    poil = poilr*pxb + a_poil;

// poilr: Wages and Prices Price of imported oil, relative to price index for bus. sector output
    (log(poilr) -
        log(poilr(-1))) = (-0.2189945671343387)*log((poilr(-1)/poilrt(-1)))+ (-0.007197716577611038)+ (0.3903451978006103)*(log((poilr(-1)))
        - log((poilr(-2))))+ (0.3940619235700209)*(log(poilrt) - log(poilrt(-1))) + a_poilr;

// ptr: Expectations 10-year expected PCE price inflation (Survey of Professional Forecasters)
    ptr = (0.9)*(ptr(-1))+ (0.05)*(picxfe(-1))+ (0.05)*(pitarg(-1)) + a_ptr;

// pxb: Wages and Prices Price index for business output
    pxb = upxb*pgdp + a_pxb;

// pxnc: Wages and Prices Price of adjusted final sales excluding consumption
    (log(pxnc) - log(pxnc(-1))) = pipxnc/400 + a_pxnc;

// pxp: Wages and Prices Price index for final sales plus imports less gov. labor
    log(pxp) = log((pxp(-1)))+ (0.6469)*log(pcnia/(pcnia(-1)))+ (0.3531)*log(pxnc/(pxnc(-1))) + a_pxp;

// pxr: Wages and Prices Price index for exports, cw (relative to PXP)
    (log(pxr) - log(pxr(-1))) = (0.0)+pipxnc/400+dpadj-(log(pxp) - log(pxp(-1))) + a_pxr;

// qebfi: Business Expenditures Desired level of investment in business investment
    qebfi = xb*vbfi*(hxbt/100-0.01*hgpbfir+jrbfi) + a_qebfi;

// qec: Household Expenditures Desired level of consumption (FRBUS definition)
    log(qec) = (0.3653279067017037)+ (0.493602432836492)*log(zyh-zyht-zyhp)+ (0.1858248429134642)*log(zyht)+ (1-(0.493602432836492)-(0.1858248429134642)-(0.1873934907158843))*log(zyhp)+ (0.1873934907158843)*log(wpo+wps)
        + a_qec;

// qecd: Household Expenditures Target level of consumption of durable goods, trending component
    qecd = qec*( jrcd/4+ (((hggdpt) + (hggdpt(-1)) + (hggdpt(-2)) + (hggdpt(-3)) + (hggdpt(-4)) +
        (hggdpt(-5)) + (hggdpt(-6)) +
        (hggdpt(-7)))/8)/400+ (-0.5843539676290892)*hgpcdr/400 )*exp((2.4132166616749)+ (-0.5843539676290892)*(log(pcdr)+log(rccd)))
        + a_qecd;

// qeco: Household Expenditures Desired level of consumption of nondurable goods and nonhousing services
    log(qeco) = (-0.3258628580621771)+log(qec/pcor) + a_qeco;

// qeh: Household Expenditures Target level of residential investment
    qeh = qec*( jrh/4+ (((hggdpt) + (hggdpt(-1)) + (hggdpt(-2)) + (hggdpt(-3)) + (hggdpt(-4)) +
        (hggdpt(-5)) + (hggdpt(-6)) + (hggdpt(-7)) + (hggdpt(-8)) + (hggdpt(-9)) + (hggdpt(-10)) +
        (hggdpt(-11)) + (hggdpt(-12)) + (hggdpt(-13)) + (hggdpt(-14)) +
        (hggdpt(-15)))/16)/400 )*exp((1.909300384501687)+ (-1.0)*log(phr*pxp/pcnia)+ (-0.1168105360328174)*log(rcch))
        + a_qeh;

// qkir: Business Expenditures Desired Inventory Sales Ratio
    log(qkir) = (1-dglprd)* (-0.001885366737710053)+log((qkir(-1))) + a_qkir;

// qlf: Labor Market Desired level of civilian labor force
    qlf = qlfpr*n16 + a_qlf;

// qlfpr: Labor Market Trend labor force participation rate
    qlfpr = (qlfpr(-1))+hqlfpr + a_qlfpr;

// qlhp: Labor Market Desired level of  business labor hours
    qlhp = xbo/lprdt + a_qlhp;

// qlww: Labor Market Trend workweek,  business sector (employee and  self-employed)
    log(qlww) = log((qlww(-1)))+(hqlww(-1))/400 + a_qlww;

// qpcnia: Wages and Prices Desired level of consumption price
    log(qpcnia) = log(qpxp*uqpct) + a_qpcnia;

// qpl: Wages and Prices Desired level of compensation per hour, trending component
    log(qpl) = log(pl)+ (1.0)*log(pxb/qpxb) + a_qpl;

// qpmo: Wages and Prices Random walk component of non-oil import prices
    log(qpmo) = log((qpmo(-1)))+ (-0.003347) + a_qpmo;

// qpxb: Wages and Prices Desired price level of private output ex. energy, housing, and farm
    log(qpxb) = log(pwstar)+ (0.0)+ (1.0)*log(pl/lprdt) + a_qpxb;

// qpxnc: Wages and Prices Desired level of nonconsumption price
    log(qpxnc) = log(pxnc)+ (2.98507462687)*log(qpxp/pxp)+ (-1.98507462687)*log(qpcnia/pcnia) + a_qpxnc;

// qpxp: Wages and Prices Desired price level of adjusted final sales
    qpxp = 100*(xpn+(0.01*qpxb*xb-xbn))/xp + a_qpxp;

// qynidn: Nominal Income Desired level of dividends
    log(qynidn) = (-0.9155533588082586) + (0.3548225925232601)*d79a + log(max(ynicpn - tcin, 0.01))
        + a_qynidn;

// rbbb: Financial Sector S&P BBB corporate bond rate
    rbbb = rbbbp+rg10 + a_rbbb;

// rbbbp: Financial Sector S&P BBB corporate bond rate, risk/term premium
    rbbbp = (1.972218)+ (-0.189051)*zgap10+ (0.848879)*((rbbbp(-1))+ (-1.972218)+ (0.189051)*(zgap10(-1)))
        + a_rbbbp;

// rbfi: Business Expenditures After-tax real financial cost of capital for business investment
    rbfi = 0.5*( 7.2+ (1-trfcim)*(rg5+rbbb-rg10)+ (-1.0)*zpib5 )+ (0.5)*req + a_rbfi;

// rcar: Financial Sector New car loan rate at finance companies
    rcar = (2.078274063162198)+ (-1.163656496933583)*d79a+ (-0.008285302552441101)*((1-d79a)*t47)+ (0.6967481719141809)*(rcar(-1))+ (0.1016693350391095)*rg5+ (0.2015824930467097)*(rg5(-1))
        + a_rcar;

// rccd: Household Expenditures Cost of capital for consumer durables
    rccd = max(100*jrcd + rcar - zpi5, 0.01) + a_rccd;

// rcch: Household Expenditures Cost of capital for residential investment
    rcch = max(100*jrh + (1-trfpm/100)*(rme + 100*trspp) - zpi10, 0.1) + a_rcch;

// rcgain: Financial Sector Rate of capital gain on the non-equity portion of household wealth
    rcgain = picx4+ (-0.4097125100056558)+ (0.4243575136593637)*xgap2+ (0.2257857751190805)*((rcgain(-1))-(picx4(-1))- (-0.4097125100056558)- (0.4243575136593637)*xgap2)
        + a_rcgain;

// req: Financial Sector Real expected rate of return on equity
    req = rg30-zpic30+reqp + a_req;

// reqp: Financial Sector Real expected rate of return on equity, premium component
    reqp = (2.903727)+ (0.808086)*rbbbp+ (0.795819)*((reqp(-1))- (2.903727)- (0.808086)*(rbbbp(-1)))
        + a_reqp;

// rff: Financial Sector Federal funds rate
    rff = (1-dmptrsh)*max(rffrule, rffmin) + dmptrsh*max(dmptr(-1)*rffrule +
        (1-dmptr(-1))*rffmin, rffmin) + a_rff;

// rffalt: Financial Sector Value of eff. federal funds rate given by estimated policy rule
    rffalt = (0.0551)+ (1.2)*(rff(-1))+ (-0.39)*(rff(-2))+ (0.6954)*xgap2+ (-0.5168)*(xgap2(-1))+ (0.3287)*(((picxfe)
        + (picxfe(-1)) + (picxfe(-2)) + (picxfe(-3)))/4) + a_rffalt;

// rffintay: Financial Sector Value of eff. federal funds rate given by the inertial Taylor rule
    rffintay = (0.85)*(rff(-1))+ (1-(0.85))*( rstar+(((picxfe) + (picxfe(-1)) + (picxfe(-2)) +
        (picxfe(-3)))/4)+ (0.5)*((((picxfe) + (picxfe(-1)) + (picxfe(-2)) +
        (picxfe(-3)))/4)-pitarg)+ (1.0)*xgap2 ) + a_rffintay;

// rffrule: Financial Sector Federal funds rate
    rffrule = dmpex*rfffix+ dmprr*( rrfix+(((picxfe) + (picxfe(-1)) + (picxfe(-2)) +
        (picxfe(-3)))/4) )+ dmptay*rfftay+ dmptlr*rfftlr+ dmpintay*rffintay+ dmpalt*rffalt +
        a_rffrule;

// rfftay: Financial Sector Value of eff. federal funds rate given by the Taylor rule with output gap
    rfftay = rstar+ (((picxfe) + (picxfe(-1)) + (picxfe(-2)) + (picxfe(-3)))/4)+ (0.5)*((((picxfe)
        + (picxfe(-1)) + (picxfe(-2)) + (picxfe(-3)))/4)-pitarg)+ (1.0)*xgap2 + a_rfftay;

// rfftlr: Financial Sector Value of eff. federal funds rate given by the Taylor rule with unemployment gap
    rfftlr = rstar+ (-0.5)*pitarg+ (0.375)*((picxfe) + (picxfe(-1)) + (picxfe(-2)) +
        (picxfe(-3)))+ (1.1)*(lurnat+deuc*leuc-lur) + a_rfftlr;

// rfynic: Foreign Trade Average yield earned on gross claims of US residents on the rest of the world
    ((rfynic) -
        (rfynic(-1))) = (0.2442546132781247)+ (-0.1404162075757351)*(rfynic(-1)-rfynil(-1))+ (0.1444243609858357)*(((rfynic(-1)))
        - ((rfynic(-2))))+ (0.6315036756054792)*((rfynil) - (rfynil(-1))) + a_rfynic;

// rfynil: Foreign Trade Average yield earned on liabilities of US residents on the rest of the world
    ((rfynil) -
        (rfynil(-1))) = (0.1769720233111774)+ (-0.2484056734487245)*(rfynil(-1))+ (0.08033864767698434)*(rg10(-1))+ (0.09023943103709648)*(rtb(-1))+ (0.04450523477009356)*(reqp(-1))+ (0.1328188190918866)*(((rfynil(-1)))
        - ((rfynil(-2))))+ (0.08760339070734239)*((rg10) - (rg10(-1)))+ (0.2614346003836546)*((rtb)
        - (rtb(-1)))+ (0.0179349568621618)*((reqp) - (reqp(-1))) + a_rfynil;

// rg10: Financial Sector 10-year Treasury bond rate
    rg10 = zrff10+rg10p + a_rg10;

// rg10p: Financial Sector 10-year Treasury bond rate, term premium
    rg10p = (1.050750815840056)+ (-0.4606588068717281)*zgap10+ (0.2287218644237361)*d8095+ (0.9201040880647524)*( (rg10p(-1))+ (-1.050750815840056)+ (0.4606588068717281)*(zgap10(-1))+ (-0.2287218644237361)*(d8095(-1)) )
        + a_rg10p;

// rg30: Financial Sector 30-year Treasury bond rate
    rg30 = zrff30+rg30p + a_rg30;

// rg30p: Financial Sector 30-year Treasury bond rate, term premium
    rg30p = (1.420673758140307)+ (-0.6248294677066961)*zgap30+ (0.1349942505218234)*d8095+ (0.9381086057081572)*( (rg30p(-1))+ (-1.420673758140307)+ (0.6248294677066961)*(zgap30(-1))+ (-0.1349942505218234)*(d8095(-1)) )
        + a_rg30p;

// rg5: Financial Sector 5-year Treasury note rate
    rg5 = zrff5+rg5p + a_rg5;

// rg5p: Financial Sector 5-year Treasury note rate. term premium
    rg5p = (0.6912438755770502)+ (-0.3495644809998469)*zgap05+ (0.9022132931202672)*( (rg5p(-1))+ (-0.6912438755770502)+ (0.3495644809998469)*(zgap05(-1)) )
        + a_rg5p;

// rgfint: Financial Sector Average rate of interest on existing federal debt
    rgfint = ( (0.86)*(rgfint(-1))+ (0.14)*(rgw(-1)) )*(gfdbtn(-2))/(gfdbtn(-1))+ (rgw(-1))*(1-(gfdbtn(-2))/(gfdbtn(-1)))+ (0.003726285406154968)
        + a_rgfint;

// rgw: Financial Sector Approximate average rate of interest on new federal debt
    rgw = (0.00495)*rtb+ (0.00271)*rg5+ (0.00129)*rg10+ (0.00105)*rg30 + a_rgw;

// rme: Financial Sector Interest rate on conventional mortgages
    ((rme) - (rme(-1))) = (0.515870426113512)+ (0.6286405556840244)*((rg10) -
        (rg10(-1)))+ (0.2555601487902845)*d87*((rg10) -
        (rg10(-1)))+ (0.2889775740676342)*((rg10(-1)-rme(-1)))+ (0.05071546489585005)*d87*((rg10(-1)-rme(-1)))
        + a_rme;

// rrff: Financial Sector Real federal funds rate
    rrff = rff-(((picxfe) + (picxfe(-1)) + (picxfe(-2)) + (picxfe(-3)))/4) + a_rrff;

// rrtr: Expectations Expected long-run real federal funds rate
    rrtr = (0.97)*(rrtr(-1))+ (0.03)*rrff + a_rrtr;

// rspnia: Nominal Income Personal saving rate
    rspnia = 100*yhsn/ydn + a_rspnia;

// rstar: Financial Sector Equilibrium real federal funds rate (for monetary policy reaction functions)
    rstar = (rstar(-1))+ (0.05)*((rrff-(rstar(-1)))*drstar) + a_rstar;

// rtb: Financial Sector 3-month Treasury bill rate
    rtb = (-0.05102548358978556)+ (0.7997187921520415)*(rtb(-1))+ (0.1113735515796583)*(rtb(-2))+ (0.7701225626671688)*rff+ (-0.6812149063988686)*(rff(-1))
        + a_rtb;

// rtbfi: Business Expenditures User cost of capital for business investment
    rtbfi = (0.01*rbfi+jrbfi-0.01*hgpbfir)*((1-0.01*tritc-trfcim*(1-tapddp*0.01*tritc)*tdpv)/(1-trfcim))*(pkbfir*pxp/pxb)
        + a_rtbfi;

// rtinv: Business Expenditures User cost of capital for inventories
    rtinv = (0.01*rbfi-0.01*hgpkir)*((pxp*pkir+(pxp(-1))*(pkir(-1)))/2)/pxb + a_rtinv;

// rtr: Expectations Expected federal funds rate in the long run (Blue Chip)
    rtr = rrtr+ptr + a_rtr;

// tcin: Government Government corporate income tax accruals, current dollar
    tcin = trci*ynicpn + a_tcin;

// tpn: Government Government personal income tax and non-tax receipts, current dollar
    tpn = trp*(ypn-gtn) + a_tpn;

// trci: Government Average government corporate income tax rate
    trci = trcit+ (0.00706626139452)*xgap2+ (0.810247648208)*((trci(-1))-(trcit(-1))+ (-0.00706626139452)*(xgap2(-1)))
        + a_trci;

// trp: Government Average government tax rate for personal income tax and non-tax receipts
    trp = (1)*trpt+ (0.603942358608)*((trp(-1)-trpt(-1)))+ (0.236576213581)*((trp(-2)-trpt(-2)))+ (0.000630587773923)*xgap2
        + a_trp;

// trpt: Government Average government tax rate for personal income tax, trend
    trpt = dfpex*trptx+ dfpdbt*( (trpt(-1))+ (0.05)*(gfdbtnp(-1)/xgdpn(-1)-gfdrt(-1))+ (0.5)*((gfdbtnp(-1)/xgdpn(-1)-gfdrt(-1))-(gfdbtnp(-2)/xgdpn(-2)-gfdrt(-2))) )+ dfpsrp*( (trpt(-1))+ (-0.1)*((gfsrpn(-1)/xgdpn(-1)-gfsrt(-1)-0.0075*xgap2(-1))) )
        + a_trpt;

// tryh: Government Average tax rate on household income
    tryh = (tpn)/(yhln+yhptn) + a_tryh;

// uqpct: Wages and Prices Stochastic component of trend ratio of PCNIA to PXP
    log(uqpct) = (0.0)+log((uqpct(-1)))+huqpct + a_uqpct;

// uxbt: Aggregate Output Identities Stochastic component of trend ratio of XGDPT to XBT
    log(uxbt) = log((uxbt(-1)))+0.0025*huxb + a_uxbt;

// uynicpnr: Nominal Income Corporate profits, residual
    uynicpnr = (-0.073960)*(1-(0.779183))+ (0.779183)*(uynicpnr(-1)) + a_uynicpnr;

// vbfi: Business Expenditures Desired investment-output ratio
    vbfi = uvbfi*(pkbfir/pbfir)/rtbfi + a_vbfi;

// wpo: Financial Sector Household property wealth ex. stock market, real
    wpo = wpon/(0.01*pcnia) + a_wpo;

// wpon: Financial Sector Household property wealth ex. stock market, current dollar
    wpon = (wpon(-1))*exp((1- (0.4106759138763171))*rcgain/400+ (0.4106759138763171)*((log(phouse)
        - log(phouse(-1)))))+ (0.25)*(ydn-ecnian-yhibn)+ (0.25)*(0.01*pcdr*pcnia*(ecd-jkcd)) +
        a_wpon;

// wps: Financial Sector Household stock market wealth, real
    wps = wpsn/(0.01*pcnia) + a_wps;

// wpsn: Financial Sector Household stock market wealth, current dollar
    log(wpsn) = log((ynicpn-tcin)*0.5)+ (-0.25)*(req-zdivgr)+ log(25)+ (1.0) + a_wpsn;

// xb: Aggregate Output Identities Business output (BEA definition), cw 2012 dollar
    xb = xbn/(pxb/100) + a_xb;

// xbn: Nominal Income Business output (BEA definition), current dollar
    xbn = pxb/100*xbo+xgdpn-xgdo*pgdp/100 + a_xbn;

// xbo: Aggregate Output Identities Business output, adjusted for measurement error, cw  2012 dollar
    log(xbo) = log(xbt)+ (1.324705489432872)*xgap2/100 + a_xbo;

// xbt: Aggregate Output Identities Potential business output, cw 2012 dollar
    log(xbt) = (0.725)*(log(leppot)+log(qlww)+log(lqualt))+ (0.275)*log(ks)+log(mfpt)+log(xbtr) + a_xbt;

// xbtr: Aggregate Output Identities Residual Factor (Potential business output)
    log(xbtr) = (0.95)*log((xbtr(-1))) + a_xbtr;

// xfs: Aggregate Output Identities Final sales of gross domestic product, cw 2012 dollar
    (log(xfs) - log(xfs(-1))) = (0.6849)*(log(ecnia) - log(ecnia(-1)))+ (0.0386)*(log(eh) -
        log(eh(-1)))+ (0.1324)*(log(ebfi) - log(ebfi(-1)))+ (0.0429)*(log(egfe) -
        log(egfe(-1)))+ (0.0223)*(log(egfl) - log(egfl(-1)))+ (0.0395)*(log(egse) -
        log(egse(-1)))+ (0.0691)*(log(egsl) - log(egsl(-1)))+ (0.1203)*(log(ex) -
        log(ex(-1)))+ (-0.1399)*(log(emo) - log(emo(-1)))+ (-0.0101)*(log(emp) - log(emp(-1))) +
        a_xfs;

// xfsn: Nominal Income Final sales of gross domestic product, current dollar
    xfsn = xgdpn-ein + a_xfsn;

// xgap: Aggregate Output Identities Output gap for  business plus oil imports  (100*(actual/potential -1)
    xgap = 100*(xbo/xbt-1) + a_xgap;

// xgap2: Aggregate Output Identities Output gap for GDP (100*(actual/potential -1)
    xgap2 = 100*(xgdo/xgdpt-1) + a_xgap2;

// xgdi: Aggregate Output Identities Gross domestic income, cw 2012 dollar
    xgdi = xgdo*mei + a_xgdi;

// xgdin: Nominal Income Gross domestic income, current dollar
    xgdin = xgdi*(pgdp/100) + a_xgdin;

// xgdo: Aggregate Output Identities Gross domestic product, adjusted for measurement error, cw 2012 dollar
    xgdo = xgdp/mep + a_xgdo;

// xgdp: Aggregate Output Identities GDP, cw 2012 dollar
    (log(xgdp) - log(xgdp(-1))) = (0.9985)*(log(xfs) - log(xfs(-1)))+ (0.6264)*(log(ki) -
        log(ki(-1)))+ (-0.6249)*(log((ki(-1))) - log((ki(-2)))) + a_xgdp;

// xgdpn: Nominal Income GDP, current dollar
    xgdpn = xpn+ein-emn+egfln+egsln + a_xgdpn;

// xgdpt: Aggregate Output Identities Potential GDP, cw 2012 dollar
    log(xgdpt) = log(xbt*uxbt) + a_xgdpt;

// xgdptn: Aggregate Output Identities Potential GDP, current dollar
    xgdptn = (0.01)*pgdp*xgdpt + a_xgdptn;

// xp: Aggregate Output Identities Final sales plus imports less government labor, cw 2012 dollar
    log(xp) = log((xp(-1)))+ (0.6526679404)*(log(ecnia) - log(ecnia(-1)))+ (0.0361108836)*(log(eh)
        - log(eh(-1)))+ (0.11825695358)*(log(ebfi) - log(ebfi(-1)))+ (0.04216893278)*(log(egfe) -
        log(egfe(-1)))+ (0.0365822346)*(log(egse) - log(egse(-1)))+ (0.114213055)*(log(ex) -
        log(ex(-1))) + a_xp;

// xpn: Nominal Income Final sales plus imports less government labor, current dollar
    xpn = (0.01)*pxp*xp + a_xpn;

// ydn: Nominal Income Disposable income
    ydn = uyd*(ypn-tpn) + a_ydn;

// yh: Nominal Income Income, household, total (real after-tax)
    yh = yhl+yht+yhp + a_yh;

// yhgap: Nominal Income Income, household, total, ratio to XGDP, cyclical component (real after-tax)
    yhgap = 100*log(yhshr/zyhst) + a_yhgap;

// yhibn: Nominal Income Consumer interest payments to business
    yhibn = uyhibn*xgdpn + a_yhibn;

// yhl: Nominal Income Income, household, labor compensation (real after-tax)
    yhl = (1-tryh)*yhln/(0.01*pcnia) + a_yhl;

// yhln: Nominal Income Income, household, labor compensation
    yhln = uyhln*(yniln) + a_yhln;

// yhp: Nominal Income Income, household, property (real after-tax)
    yhp = ((1-tryh)*yhptn+yhpntn)/(0.01*pcnia) + a_yhp;

// yhpcd: Household Expenditures Imputed income of the stock of consumer durables, 2012 dollar
    log(yhpcd) = log(0.053750)+log((kcd(-1))) + a_yhpcd;

// yhpgap: Nominal Income Income, household, property, ratio to YH, cyclical component (real after-tax)
    yhpgap = 100*log(yhpshr/zyhpst) + a_yhpgap;

// yhpntn: Nominal Income Income, household, property, non-taxable component
    yhpntn = (0.01)*pcnia*pcdr*yhpcd+ (-1.0)*yhibn+ynicpn+ (-1.0)*tcin-ynidn+ (-0.01)*zpi10*gfdbtn
        + a_yhpntn;

// yhpshr: Nominal Income Income, household, property, ratio to YH (real after-tax)
    yhpshr = yhp/yh + a_yhpshr;

// yhptn: Nominal Income Income, household, property, taxable component
    yhptn = uyhptn*(ynirn+gfintn+ynidn+yhibn) + a_yhptn;

// yhshr: Nominal Income Income, household, total, ratio to XGDP (real after-tax)
    yhshr = yh/xgdp + a_yhshr;

// yhsn: Nominal Income Personal saving
    yhsn = yhln+yhtn+yhptn-tpn-ecnian-yhibn+uyhsn*xgdptn + a_yhsn;

// yht: Nominal Income Income, household, transfer (real after-tax), net basis
    yht = yhtn/(0.01*pcnia) + a_yht;

// yhtgap: Nominal Income Income, household, transfer, ratio to YH, cyclical component (real after-tax)
    yhtgap = 100*log(yhtshr/zyhtst) + a_yhtgap;

// yhtn: Nominal Income Income, household, transfer payments. net basis
    yhtn = uyhtn*gtn + a_yhtn;

// yhtshr: Nominal Income Income, household, transfer, ratio to YH (real after-tax)
    yhtshr = yht/yh + a_yhtshr;

// ykbfin: Nominal Income Income from stock of BFI
    ykbfin = (0.01)*rtbfi*pxb*(kbfi+(kbfi(-1)))/2 + a_ykbfin;

// ykin: Nominal Income Income from stock of inventories
    ykin = (0.01)*rtinv*pxb*(ki+(ki(-1)))/2 + a_ykin;

// ynicpn: Nominal Income Corporate profits (national income component)
    ynicpn = max(ynin - yniln - ynirn + uynicpnr*xgdpn, tcin + 0.01*xgdpn) + a_ynicpn;

// ynidn: Nominal Income Dividends (national income component)
    ynidn = ymsdn+ exp(log(pxb)+ log(((ynidn(-1)-ymsdn(-1))/pxb(-1)))+ (0.1072695065113163)*log((qynidn(-1)/(ynidn(-1)-ymsdn(-1))))+ (-0.2095634314107719)*(log(((ynidn(-1)-ymsdn(-1))/pxb(-1)))-log(((ynidn(-2)-ymsdn(-2))/pxb(-2))))+ (1.0)*zynid)
        + a_ynidn;

// yniln: Nominal Income Labor income (national income component)
    yniln = (0.01)*uyl*(pl*lhp+pgfl*egfl+pgsl*egsl) + a_yniln;

// ynin: Nominal Income National income
    ynin = uyni*(xgdin+fynin-jccan) + a_ynin;

// ynirn: Government Net interest, rental and proprietors' incomes (national income components)
    ynirn = xgdpn*( (0.007593)+ (0.944044)*(ynirn(-1)/xgdpn(-1))+ (0.074817)*0.01*((rbbb) -
        (rbbb(-1))) ) + a_ynirn;

// ypn: Nominal Income Personal income
    ypn = uyp*(yhln+yhtn+yhptn) + a_ypn;

// zdivgr: Expectations Expected growth rate of real dividends, for WPSN eq. (VAR exp.)
    zdivgr = (0.009757264257434617)*(hgynid(+1))+ (0.9902427357425654)*(zdivgr(+1)) + a_zdivgr;

// zebfi: Expectations Expected growth rate of business output EBFI (VAR exp.)
    zebfi = (-0.0000000000000001836241195103586)+ (-0.0004311442119548698)*(picnia(-1))+ (-0.0005071417360301063)*(picnia(-2))+ (-0.00003881819160880394)*(picnia(-3))+ (0.0001679875754395107)*(picnia(-4))+ (-0.0009752514829426216)*(rff(-1))+ (0.0004172696850180997)*(rff(-2))+ (0.000009804022481480238)*(rff(-3))+ (0.0004025448938499502)*(rff(-4))+ (0.0001456328815931833)*(rtr(-1))+ (0.0008091165641542619)*(ptr(-1))+ (0.0006914817407124366)*(xgap(-1))+ (-0.001524629901125206)*(xgap(-2))+ (0.0001821021224146923)*(xgap(-3))+ (0.0001709602428972692)*(xgap(-4))+ (0.01429456576547675)*(log((qebfi(-1)))
        - log((qebfi(-2))))+ (0.01004233676573099)*(log((qebfi(-2))) -
        log((qebfi(-3))))+ (0.005011839434648968)*(log((qebfi(-3))) -
        log((qebfi(-4))))+ (0.003887436951496095)*(log((qebfi(-4))) -
        log((qebfi(-5))))+ (0.1422818153960116)*(hxbt(-1))/400+ (-0.1422818153960531)*(hgpbfir(-1))/400
        + a_zebfi;

// zecd: Expectations Expected growth rate of target durable consumption, for ECD eq. (VAR exp.)
    zecd = (-0.0004244330449108246)*(picnia(-1))+ (-0.000566112732916264)*(picnia(-2))+ (-0.0004278354154853733)*(picnia(-3))+ (0.000004275450618659433)*(picnia(-4))+ (-0.001333637468405136)*(rff(-1))+ (0.001785102754320539)*(rff(-2))+ (-0.0002714744059753036)*(rff(-3))+ (0.0004596118643773856)*(rff(-4))+ (0.0004286088490690454)*(xgap2(-1))+ (-0.001112480888047013)*(xgap2(-2))+ (0.00003611331309391688)*(xgap2(-3))+ (0.00007975907057934714)*(xgap2(-4))+ (0.001414105742693884)*(ptr(-1))+ (-0.0006396027443177932)*(rtr(-1))+ (-0.0001084142645104688)*(yhgap(-1))+ (0.0002103631242011464)*(yhgap(-2))+ (0.0001780616641343914)*(yhgap(-3))+ (0.0001469127491672944)*(yhgap(-4))+ (-0.0001398804267539158)*(yhtgap(-1))+ (-0.00003380075732963865)*(yhtgap(-2))+ (0.0001669757937064113)*(yhtgap(-3))+ (0.0001135069368207403)*(yhtgap(-4))+ (0.0001241231278850025)*(yhpgap(-1))+ (-0.0002035919714857632)*(yhpgap(-2))+ (0.00005798918819304864)*(yhpgap(-3))+ (0.0001142807758711503)*(yhpgap(-4))+ (1.020354953788767)*(hggdpt(-1))/400+ (-0.752244722798112)*(hgpcdr(-1))/400+ (0.030859810575484)*(log((qecd(-1)))
        - log((qecd(-2))))+ (0.0288465643523214)*(log((qecd(-2))) -
        log((qecd(-3))))+ (-0.007704793774557718)*(log((qecd(-3))) -
        log((qecd(-4))))+ (-0.01236375512939394)*(log((qecd(-4))) - log((qecd(-5)))) + a_zecd;

// zeco: Expectations Expected growth rate of target nondurables and nonhousing services, for ECO eq (VAR
    zeco = (-0.0000752202049495858)*(picnia(-1))+ (-0.00007944069331813561)*(picnia(-2))+ (-0.00002059316996140204)*(picnia(-3))+ (0.0001004397794984203)*(picnia(-4))+ (0.00002128321856982191)*(rff(-1))+ (0.00001703531535880844)*(rff(-2))+ (0.00005501237638104223)*(rff(-3))+ (0.0000368085672111148)*(rff(-4))+ (-0.0006301710369219651)*(xgap2(-1))+ (0.0002738755865138929)*(xgap2(-2))+ (0.0001330197561306166)*(xgap2(-3))+ (-0.00003466191405309875)*(xgap2(-4))+ (0.00007481428873071481)*(ptr(-1))+ (-0.0001301394775207934)*(rtr(-1))+ (-0.000574849476125713)*(yhgap(-1))+ (0.000315791553755267)*(yhgap(-2))+ (0.0003970054362973829)*(yhgap(-3))+ (0.00002606365933680952)*(yhgap(-4))+ (-0.00006065913885267499)*(yhtgap(-1))+ (-0.000005861516974911094)*(yhtgap(-2))+ (0.00004608692992415466)*(yhtgap(-3))+ (-0.00003675339093791547)*(yhtgap(-4))+ (0.0002055017720241992)*(yhpgap(-1))+ (-0.0002409377143989024)*(yhpgap(-2))+ (-0.0001318122876593039)*(yhpgap(-3))+ (-0.00008998120362836395)*(yhpgap(-4))+ (0.4694132786802296)*(hggdpt(-1))/400+ (0.07322397247249938)*(log((qeco(-1)))
        - log((qeco(-2))))+ (0.02932374759217591)*(log((qeco(-2))) -
        log((qeco(-3))))+ (0.00714849213690236)*(log((qeco(-3))) -
        log((qeco(-4))))+ (0.00907784707291976)*(log((qeco(-4))) - log((qeco(-5)))) + a_zeco;

// zeh: Expectations Expected growth rate of target residential investment, for EH eq. (VAR exp.)
    zeh = (-0.00007825906447294479)*(picnia(-1))+ (-0.00005221776500989767)*(picnia(-2))+ (-0.0000511617748264036)*(picnia(-3))+ (0.00001061188019850692)*(picnia(-4))+ (0.0001524702142702607)*(rff(-1))+ (0.0001154619164109844)*(rff(-2))+ (0.0000694775905299206)*(rff(-3))+ (0.000006915284271997218)*(rff(-4))+ (0.0005016959759548423)*(xgap2(-1))+ (-0.0005314894632374061)*(xgap2(-2))+ (-0.0001527167228830226)*(xgap2(-3))+ (0.00004422297252718166)*(xgap2(-4))+ (0.0001710267241107452)*(ptr(-1))+ (-0.0003443250054832286)*(rtr(-1))+ (0.00005547462567554765)*(yhgap(-1))+ (0.00004078231691046518)*(yhgap(-2))+ (-0.00003078619404779462)*(yhgap(-3))+ (-0.000004584642539780123)*(yhgap(-4))+ (0.000007136415206693365)*(yhtgap(-1))+ (0.00001470739436481251)*(yhtgap(-2))+ (0.00003830847423082347)*(yhtgap(-3))+ (0.00003271967551086854)*(yhtgap(-4))+ (-0.00006992854917184709)*(yhpgap(-1))+ (0.0000004550096611877513)*(yhpgap(-2))+ (0.00005504613762415508)*(yhpgap(-3))+ (0.00002864192627076078)*(yhpgap(-4))+ (0.4277729262319505)*(hggdpt(-1))/400+ (0.004266303023854793)*(log((qeh(-1)))
        - log((qeh(-2))))+ (-0.001472171716551593)*(log((qeh(-2))) -
        log((qeh(-3))))+ (-0.003348261838736116)*(log((qeh(-3))) -
        log((qeh(-4))))+ (-0.004008022551322911)*(log((qeh(-4))) - log((qeh(-5)))) + a_zeh;

// zgap05: Expectations Expected output gap, for RG5 eq. (VAR exp.)
    zgap05 = (0.05479365264340726)*xgap+ (0.9452063473565927)*(zgap05(+1)) + a_zgap05;

// zgap10: Expectations Expected output gap, for RG10 eq. (VAR exp.)
    zgap10 = (0.03007455810944507)*xgap+ (0.969925441890555)*(zgap10(+1)) + a_zgap10;

// zgap30: Expectations Expected output gap, for RG30 eq. (VAR exp.)
    zgap30 = (0.01410658898201937)*xgap+ (0.9858934110179807)*(zgap30(+1)) + a_zgap30;

// zgapc2: Expectations Expected output gap, for ECD eq. (VAR exp.)
    zgapc2 = (-0.01418483319860699)*(picnia(-1))+ (-0.00438957847118337)*(picnia(-2))+ (-0.006089864990632572)*(picnia(-3))+ (0.001274535866759684)*(picnia(-4))+ (-0.0426889990257948)*(rff(-1))+ (0.007759946050458921)*(rff(-2))+ (0.01912856687923288)*(rff(-3))+ (-0.002207952775923244)*(rff(-4))+ (0.1943845369683129)*(xgap2(-1))+ (-0.07640072342643054)*(xgap2(-2))+ (-0.01132460234847898)*(xgap2(-3))+ (-0.01555183396619014)*(xgap2(-4))+ (0.02338974079366426)*(ptr(-1))+ (0.01800843887202016)*(rtr(-1))
        + a_zgapc2;

// zlhp: Expectations Expected growth rate of target aggregate hours (VAR exp.)
    zlhp = (-0.0002023213774340967)*(picnia(-1))+ (-0.00006547091555559659)*(picnia(-2))+ (-0.0001720246830139474)*(picnia(-3))+ (0.0000313937564958239)*(picnia(-4))+ (-0.001047476022547437)*(rff(-1))+ (0.0002598839064585707)*(rff(-2))+ (0.0005079095801600575)*(rff(-3))+ (-0.0000419862687487686)*(rff(-4))+ (0.0003216688046775128)*(rtr(-1))+ (0.0004084232195080358)*(ptr(-1))+ (-0.005754964729358652)*(xgap(-1))+ (0.005816827692544119)*(xgap(-2))+ (-0.0002034859294983795)*(xgap(-3))+ (-0.0002715434817101584)*(xgap(-4))+ (0.6857946901749924)*((log((xbo(-1)))
        - log((xbo(-2))))-(log((lprdt(-1))) -
        log((lprdt(-2)))))+ (0.1113240674326222)*(hlept(-1)+hqlww(-1))/400 + a_zlhp;

// zpi10: Expectations Expected cons. price infl., for RCCH, RRMET, and YHPNTN eqs. (10-yr mat.) (VAR exp.)
    zpi10 = (0.03007455810944507)*picnia+ (0.969925441890555)*(zpi10(+1)) + a_zpi10;

// zpi10f: Expectations Expected cons. price infl., for FPXR eq. (10-yr mat.) (VAR exp.)
    zpi10f = (0.03007455810944507)*picnia+ (0.969925441890555)*(zpi10f(+1)) + a_zpi10f;

// zpi5: Expectations Expected cons. price infl., for RCCD eq. (5-yr mat.) (VAR exp.)
    zpi5 = (0.08178762749631196)*(picnia(-1))+ (0.02218684188675673)*(picnia(-2))+ (0.02501945218256163)*(picnia(-3))+ (-0.00009007062448078559)*(picnia(-4))+ (-0.1456765471759016)*(rff(-1))+ (-0.03113773606787808)*(rff(-2))+ (-0.02949319295743922)*(rff(-3))+ (-0.0275798582145864)*(rff(-4))+ (0.2338873344157305)*(rtr(-1))+ (0.8710961490588724)*(ptr(-1))+ (0.174192252057006)*(xgap(-1))+ (-0.07184023126885179)*(xgap(-2))+ (0.04066371951581198)*(xgap(-3))+ (0.04494462398507848)*(xgap(-4))
        + a_zpi5;

// zpib5: Expectations Expected output price infl., for RPD eq. (5-yr mat.) (VAR exp.)
    zpib5 = (0.05479365264340726)*400*(log(pxb) - log(pxb(-1)))+ (0.9452063473565927)*(zpib5(+1)) +
        a_zpib5;

// zpic30: Expectations Expected cons. price infl., for REQ eq. (30-yr mat.) (VAR exp.)
    zpic30 = (0.01410658898201937)*picnia+ (0.9858934110179807)*(zpic30(+1)) + a_zpic30;

// zpic58: Expectations Expected 4-qtr consumer price inflation (8 qtrs. in the future) (VAR exp.)
    zpic58 = (pic4(+8)) + a_zpic58;

// zpicxfe: Expectations Expected value of picxfe in the next quarter (VAR exp.)
    zpicxfe = (picxfe(+1)) + a_zpicxfe;

// zpieci: Expectations Expected value of pieci in the next quarter (VAR exp.)
    zpieci = (pieci(+1)) + a_zpieci;

// zrff10: Expectations Expected federal funds rate, for RG10 eq. (10-yr mat.) (VAR exp.)
    zrff10 = (0.03007455810944507)*rff+ (0.969925441890555)*(zrff10(+1)) + a_zrff10;

// zrff30: Expectations Expected federal funds rate, for RG30 eq. (30-yr mat.) (VAR exp.)
    zrff30 = (0.01410658898201937)*rff+ (0.9858934110179807)*(zrff30(+1)) + a_zrff30;

// zrff5: Expectations Expected federal funds rate, for RG5 eq. (5-yr mat.) (VAR exp.)
    zrff5 = (0.05479365264340726)*rff+ (0.9452063473565927)*(zrff5(+1)) + a_zrff5;

// zyh: Expectations Expected level of real after-tax household income, for QEC eq. (VAR exp.)
    log(zyh) = (0.00008403034216398836)*picnia+ (0.0007023129900742578)*(picnia(-1))+ (0.0005988312485600414)*(picnia(-2))+ (0.00046906873529373)*(picnia(-3))+ (-0.002112400975583133)*rff+ (0.0001656602252725924)*(rff(-1))+ (-0.0007780393043584456)*(rff(-2))+ (0.0002906813774138189)*(rff(-3))+ (0.0009057626374626405)*xgap2+ (0.002687425542518095)*(xgap2(-1))+ (0.0006823597205465298)*(xgap2(-2))+ (-0.001256388702977828)*(xgap2(-3))+ (-0.001854243316194259)*ptr+ (0.002434098677388386)*rtr+ (0.004161665385029935)*yhgap+ (0.0007501557850814375)*(yhgap(-1))+ (-0.0001921235465779609)*(yhgap(-2))+ (-0.0003602829076187046)*(yhgap(-3))+ (0.9999999999999565)*log(zyhst*xgdpt)
        + a_zyh;

// zyhp: Expectations Expected level of real after-tax property income, for QEC eq. (VAR exp.)
    log(zyhp) = (0.0008637359958914742)*picnia+ (0.001043993924986531)*(picnia(-1))+ (0.0009412688975392932)*(picnia(-2))+ (0.000458916642717137)*(picnia(-3))+ (-0.001544432205587824)*rff+ (-0.0006407725735247019)*(rff(-1))+ (-0.000877216982815245)*(rff(-2))+ (-0.00004075028532698682)*(rff(-3))+ (-0.002036934910319444)*xgap2+ (0.003079022876028106)*(xgap2(-1))+ (0.001533670579319286)*(xgap2(-2))+ (-0.0009794433698078088)*(xgap2(-3))+ (-0.003307915461364551)*ptr+ (0.003103172047474443)*rtr+ (0.004220697583687526)*yhgap+ (0.0001307621492671122)*(yhgap(-1))+ (0.00005342070875074808)*(yhgap(-2))+ (-0.0008349442433696925)*(yhgap(-3))+ (0.002251478648554251)*yhpgap+ (0.0002014050026037725)*(yhpgap(-1))+ (-0.0004274985452560566)*(yhpgap(-2))+ (-0.0002520786346265581)*(yhpgap(-3))+ (0.9999999999999468)*log(zyhpst*zyhst*xgdpt)
        + a_zyhp;

// zyhpst: Expectations Expected trend share of property income in household income
    log(zyhpst) = log((zyhpst(-1)))+ (0.05)*(yhpgap(-1))/100 + a_zyhpst;

// zyhst: Expectations Expected trend ratio of household income to GDP
    log(zyhst) = log((zyhst(-1)))+ (0.05)*(yhgap(-1))/100 + a_zyhst;

// zyht: Expectations Expected level of real transfer income, for QEC eq. (VAR exp.)
    log(zyht) = (-0.000334830912493238)*picnia+ (0.0004734682680155963)*(picnia(-1))+ (0.0002798072585528634)*(picnia(-2))+ (0.000327960879431146)*(picnia(-3))+ (-0.002500279763979537)*rff+ (0.0008855681526303969)*(rff(-1))+ (-0.001214881268111102)*(rff(-2))+ (0.00008524329709881322)*(rff(-3))+ (0.002172842548066125)*xgap2+ (0.00313028920932204)*(xgap2(-1))+ (0.002114206871939033)*(xgap2(-2))+ (0.0002481445696860439)*(xgap2(-3))+ (-0.0007464054935481564)*ptr+ (0.002744349582394016)*rtr+ (0.002708245687651875)*yhgap+ (0.0006793788749861876)*(yhgap(-1))+ (0.0003089861693518611)*(yhgap(-2))+ (-0.00006935116601571522)*(yhgap(-3))+ (0.001837608113293888)*yhtgap+ (0.0005096499176820247)*(yhtgap(-1))+ (0.00009470842999385746)*(yhtgap(-2))+ (0.00024263888389989)*(yhtgap(-3))+ (0.9999999999999932)*log(zyhtst*zyhst*xgdpt)
        + a_zyht;

// zyhtst: Expectations Expected trend share of transfer income in household income
    log(zyhtst) = log((zyhtst(-1)))+ (0.05)*(yhtgap(-1))/100 + a_zyhtst;

// zynid: Expectations Expected rate of growth of target real dividends, for YNIDN eq. (VAR exp.)
    zynid = (0.0000000000000005206855405139395)+ (-0.0001020778460719867)*(picnia(-1))+ (0.0003486952052516843)*(picnia(-2))+ (0.0002522503063284875)*(picnia(-3))+ (0.0002059799369104374)*(picnia(-4))+ (0.000352649102886596)*(rff(-1))+ (-0.0009117152893301089)*(rff(-2))+ (-0.0008332522818027586)*(rff(-3))+ (0.0002220282051739009)*(rff(-4))+ (0.001170290263072301)*(rtr(-1))+ (-0.0007048476024184936)*(ptr(-1))+ (-0.005098524468651768)*(xgap(-1))+ (0.001661127416240023)*(xgap(-2))+ (0.0006345562668170024)*(xgap(-3))+ (0.001396819940195254)*(xgap(-4))+ (0.002519112167658877)*(log((qynidn(-1)/pxb(-1)))
        - log((qynidn(-2)/pxb(-2))))+ (0.01551673287897806)*(log((qynidn(-2)/pxb(-2))) -
        log((qynidn(-3)/pxb(-3))))+ (0.01239948969603592)*(log((qynidn(-3)/pxb(-3))) -
        log((qynidn(-4)/pxb(-4))))+ (-0.006974010098893951)*(log((qynidn(-4)/pxb(-4))) -
        log((qynidn(-5)/pxb(-5))))+ (1.186101463663362)*(hggdpt(-1))/400 + a_zynid;
