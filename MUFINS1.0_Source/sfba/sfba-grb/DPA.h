/* 
 * File:   DPA.h
 * Author: John
 *
 * Created on 15 September 2011, 10:38
 */

#ifndef DPA_H
#define	DPA_H

#include "MtxLP.h"
#include "SplitLP.h"
#include "TxtParser.h"


class DPA {

//private:
public:
    strvec genes;// all genes of model
    strvec arrName;// array names
    strstr2flo arrexp;//w/ for float ratio expression
    gene_reactions geneR; // gene to reactions for current model
    met_genes metG;//metabolite map to genes
    strvec mets;//metabolites
    

public:
    DPA();
    ~DPA();

    void read_arrayExpression(string expfile, string err_msg);
    void read_dpaplot(string dpafile, string err_msg);
    void gene_koreacs(Model* model);
    void koGenes(strvec &rules, int k1, int k2, string Rn);
    strvec getRkos(string Gn){return geneR[Gn];};
    void ko_mlp(MtxLP* blp, strvec rkos, kobds &ko_rc);
    void rko_mlp(MtxLP* mlp, kobds &ko_rc);
    RC set_korc(MtxLP* lp, string name, bool c);
    double getMedian(flovec vec);
    void compute_signal(strvec &arrs, metarr_sig &metsig);

};



#endif	/* DPA_H */

