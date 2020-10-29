#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Dec 12 13:42:10 2017

@author: rve
"""


# =============================================================================
# Download Gene Expression Omnibus Database (GEO) if there is not a local file
# https://www.ncbi.nlm.nih.gov/
#
# Example:
# gds = dnld_GEO('GDS750', './Datasets/')
# =============================================================================
def dnld_GEO(geo_name, path_dest):
    import GEOparse
    geo_obj = GEOparse.get_GEO(geo=geo_name, destdir=path_dest)
    return geo_obj


# =============================================================================
# Read a GAF file and return a dictionary mapping Genes to Go-Terms
#
# Ontologies can be downloaded at
# http://geneontology.org/page/download-ontology
# GAF files can be downloaded at
# http://geneontology.org/page/download-annotations
#
# Example:
#from rve_goeaTools import getAssociations
#ns2assoc = getAssociations(path_gaf='./GAFs/gene_association.sgd')
# =============================================================================
def getAssociations(path_gaf):
    from goatools.anno.gaf_reader import GafReader
    gaf = GafReader(path_gaf)
    ns2assoc = {'BP': {}, 'CC': {}, 'MF':{}} # BP: biological_process; MF: molecular_function; CC: cellular_component
    for ntgaf in gaf.associations:
        assoc = ns2assoc[ntgaf.NS]
        gene = ntgaf.DB_Symbol
        goid = ntgaf.GO_ID
        if gene not in assoc:
            assoc[gene] = set()
        assoc[gene].add(goid)
    return ns2assoc
