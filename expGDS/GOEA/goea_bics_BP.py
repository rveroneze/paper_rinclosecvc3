#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Oct 27 10:41:56 2020

@author: rve
"""

#Code based on:
#https://github.com/tanghaibao/goatools/blob/main/notebooks/goea_nbt3102.ipynb

thr = 0.05 # default significance cut-off

# Reading the biclustering output:
bics = []
with open('GDS181r631c3e41.py') as bicfile:
    for line in bicfile:
        exec(line)
# ----

from goatools.obo_parser import GODag
obodag = GODag('./GO/go-basic.obo')


import rve_goeaTools
ns2assoc = rve_goeaTools.getAssociations(path_gaf='./GAFs/goa_human.gaf')


gds = rve_goeaTools.dnld_GEO('GDS181', './Datasets/')
genes = gds.table.IDENTIFIER.to_list();


from goatools.goea.go_enrichment_ns import GOEnrichmentStudy
goeaobj = GOEnrichmentStudy(
        genes, # list of genes
        ns2assoc['BP'], # geneid/GO associations
        obodag, # Ontologies
        propagate_counts = False,
        alpha = thr, # default significance cut-off
        methods = ['fdr_bh']) # defult multipletest correction method

sizer = []
sizec = []
goea_nsig = []
goea_bestp = []
i = 0
for bic in bics:
    print(i)
    rows = bic[0]
    cols = bic[1]
    geneids_study = [genes[row] for row in rows]
    goea_results_all = goeaobj.run_study(geneids_study, prt=None) # prt=None: for quietly run GOEA
    nsig = 0
    bestp = 100
    for r in goea_results_all:
        if r.p_fdr_bh < thr:
            nsig = nsig + 1
        if r.p_fdr_bh < bestp:
            bestp = r.p_fdr_bh
    sizer.append(len(rows))
    sizec.append(len(cols))
    goea_nsig.append(nsig)
    goea_bestp.append(bestp)
    i = i + 1


import pandas as pd
df = pd.DataFrame(list(zip(sizer, sizec, goea_nsig, goea_bestp)),columns=['sizer', 'sizec', 'goea_nsig', 'goea_bestp'])
df.to_csv('GDS181r631c3e41_BP.csv')


print("Total number of bics = " + str(len(bics)))
print("Number of enriched bics = " + str(len(goea_nsig) - goea_nsig.count(0)))
print("% of enriched bics = " + str((len(goea_nsig) - goea_nsig.count(0)) / len(goea_nsig) * 100))



import numpy as np

best_bic = np.argmin(goea_bestp)
rows = bics[best_bic][0]
geneids_study = [genes[row] for row in rows]
goea_results_all = goeaobj.run_study(geneids_study)
goea_results_sig = [r for r in goea_results_all if r.p_fdr_bh < thr]
goeaobj.wr_xlsx("GDS181r631c3e41_BP_bestP.xlsx", goea_results_sig)


best_bic = np.argmax(goea_nsig)
rows = bics[best_bic][0]
geneids_study = [genes[row] for row in rows]
goea_results_all = goeaobj.run_study(geneids_study)
goea_results_sig = [r for r in goea_results_all if r.p_fdr_bh < thr]
goeaobj.wr_xlsx("GDS181r631c3e41_BP_bestN.xlsx", goea_results_sig)
