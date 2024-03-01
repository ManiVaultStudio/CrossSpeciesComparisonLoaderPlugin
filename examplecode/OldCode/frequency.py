import pandas as pd
speciesNamelist=['Human', 'Chimpanzee', 'Gorilla', 'Rhesus', 'Marmoset']
#speciesName='Human'# Chimpanzee, Gorilla, Rhesus, Marmoset
labelValuelist=['subclass_label','neighborhood_label','cross_species_subclass_label','cross_species_cluster_label','cluster_label','class_label']
#labelValue='cross_species_cluster_label'

for labelValue in labelValuelist:
    for speciesName in speciesNamelist:
        df=pd.read_csv("Simian-"+speciesName+"-10xMetadata.csv")
        count_result = df.groupby(labelValue).size()
        count_result_df = count_result.reset_index(name=speciesName)
        count_result_df.to_csv("output/frequencyValues"+labelValue+"_"+speciesName+".csv",index=None)
    dfChimpanzee=pd.read_csv("output/frequencyValues"+labelValue+"_Chimpanzee.csv")
    dfGorilla=pd.read_csv("output/frequencyValues"+labelValue+"_Gorilla.csv")
    dfHuman=pd.read_csv("output/frequencyValues"+labelValue+"_Human.csv")
    dfMarmoset=pd.read_csv("output/frequencyValues"+labelValue+"_Marmoset.csv")
    dfRhesus=pd.read_csv("output/frequencyValues"+labelValue+"_Rhesus.csv")
    merged_df = pd.merge(dfChimpanzee, dfGorilla, on=labelValue, how='outer')
    merged_df = pd.merge(merged_df, dfHuman, on=labelValue, how='outer')
    merged_df = pd.merge(merged_df, dfMarmoset, on=labelValue, how='outer')
    merged_df = pd.merge(merged_df, dfRhesus, on=labelValue, how='outer')
    columns = merged_df.columns.tolist()
    columns = columns[1:] + [columns[0]]
    merged_df = merged_df[columns]
    merged_df.fillna(0, inplace=True)
    merged_df.to_csv("frequencyfiles/frequency_"+labelValue+".csv",index=None)