#include "DPA.h"

DPA::DPA(){
}
DPA::~DPA(){}


void DPA::read_arrayExpression(string expfile, string err_msg){ //w/ read gene/enzyme expression file
    //parsing the input
    if (!fexists(expfile))
        throw runtime_error(err_msg + "no expression file specified!");
    parse_arrayExpression_f(expfile, arrName, arrexp);
}

void DPA::read_dpaplot(string dpafile, string err_msg){//read dpaplot output to get map of metabolite to genes
    //parsing the input
    if (!fexists(dpafile))
        throw runtime_error(err_msg + "no dpaplot file specified!");
    parse_dpaplot(dpafile, mets, metG);
}

// gene to its knockout Reactions map
void DPA::gene_koreacs(Model* model){
    strvec reacs=model->get_reactions();
    for(int i=0;i<reacs.size();i++){
        string Rn=reacs[i];
        Reaction* reac=model->get_reac(Rn);
        strvec rules=reac->get_rules();
        if(!rules.empty()){ //if OR in Rule,then donot map all genes for this reac
            koGenes(rules, 0, rules.size(), Rn);
        }
    }
}

void DPA::koGenes(strvec &rules, int k1, int k2, string Rn){
    for(int i=k1;i<k2;i++){
        string rs=rules[i];
        if(rs=="("){
            int p=1;
            for(int j=i+1;j<k2;j++){
                if(rules[j]=="(") p++;
                else if(rules[j]==")"){
                    p--;
                    if(p!=0) p--;
                    else if(p==0) {
                        if(i>0?rules[i-1]==AND:1 && j+1<k2?rules[j+1]==AND:1)
                            koGenes(rules, i+1,j, Rn);
                        i=j+1;
                        break;
                    }
                }
            }
        }
        else if(rs!=AND && rs!=OR){
            if(i>k1?rules[i-1]==AND:1 && i+1<k2?rules[i+1]==AND:1){
                if(geneR.find(rs)==geneR.end()) genes.push_back(rs);
                if(find(geneR[rs].begin(),geneR[rs].end(),Rn)==geneR[rs].end()) //avoid repeated genes for a reaction
                    geneR[rs].push_back(Rn);
            }
        }
    }
}


void DPA::ko_mlp(MtxLP* lp, strvec rkos, kobds &ko_rc){//gni/knockout mlp model
//    strvec Rt;
     for(int i=0;i<rkos.size();i++){
        string Rn=rkos[i];
//        if(find(Rt.begin(),Rt.end(),Rn)==Rt.end()){//avoid the problem of reapeat reactions for a gene
//            Rt.push_back(Rn);
            ko_rc[Rn]=set_korc(lp, Rn, 1);
            lp->delCol(Rn);
//        }
    }
}

void DPA::rko_mlp(MtxLP* lp, kobds &ko_rc){//gni/
    for(kobds::iterator it=ko_rc.begin();it!=ko_rc.end();it++){
        string name=it->first;
        RC rc=it->second;
        lp->addRC(name, rc);
    }
}

RC DPA::set_korc(MtxLP* lp, string name, bool c){
    RC rc;
    if(c==1){
        rc.col=1;
        rc.lb=lp->getColLB(name);
        rc.ub=lp->getColUB(name);
        rc.sto=lp->getColSto(name);
    }else if(c==0){
        rc.col=0;
        rc.lb=lp->getRowLB(name);
        rc.ub=lp->getRowUB(name);
        rc.sto=lp->getRowSto(name);
    }
    return rc;
}

double DPA::getMedian(flovec vec){
    int size=vec.size();
    double median;
    double mid=(float)size/2-1;
    sort(vec.begin(),vec.end());
    if (size % 2) median=vec[mid+0.5];
    else median = (vec[mid]+vec[mid + 1])/2;
    return median;
}

void DPA::compute_signal(strvec &arrs, metarr_sig &metsig){
    for (met_genes::iterator it = metG.begin(); it != metG.end(); ++it){
        string met=it->first;
        strvec mgenes=it->second;
        for(int i=0;i<arrs.size();i++){
            flovec upGns, dwGns;
            string arr=arrs[i];
            for(int j=0;j<mgenes.size();j++){
                string Gn=mgenes[j];
                if(arrexp.find(Gn)!=arrexp.end()){
                    double Ratio=arrexp[Gn][arr];
                    if(Ratio>0 && Ratio!=100) upGns.push_back(Ratio); //100 : 'NA'
                    else if(Ratio<0 && Ratio!=1e5) dwGns.push_back(Ratio);
                } 
            }
            metsig.upsig[met][arr]=upGns.size()>0?getMedian(upGns):0;
            metsig.dwsig[met][arr]=dwGns.size()>0?getMedian(dwGns):0;
            upGns.clear();
            dwGns.clear();
        }
    }
}