In der Vorlesung ist der sequentielle Algorithmus von Dijkstra vorgestellt und diskutiert worden („single source shortest paths“). Implementieren Sie eine parallele Version des Verfahrens mit den bisher
bekannt Programmiermethoden (entweder direkt mit C++17 und/oder mit der TBB-Bibliothek). Bestimmen Sie den Parallelitätsgewinn (Speed-Up) und die Effizienz.
Für die Verwendung der oneAPI-Bibliothek1 sind die Umgebungsvariablen entsprechend zu setzen:
'''bash
source /opt/intel/oneapi/setvars.sh
'''
Der C++-Namensraum wird durch oneapi::tbb:: angegeben. Die Dokumentation zur Bibliothek ist
zu finden unter:
https://spec.oneapi.io/versions/1.1-rev-1/elements/oneTBB/source/nested-index.html
Als Eingabedateien verwenden Sie bitte die Beispieldateien der DIMACS-Challenge unter der URL
http://www.diag.uniroma1.it//~challenge9/download.shtml. Als Eingabeformat wird das „Graph Specification Format“ (http://www.diag.uniroma1.it//~challenge9/format.shtml) in Form einer .gr-Datei
verwendet. Die Ausgabe soll im Gegensatz zu dem o.g. Wettbewerb dazu nur auf dem Bildschirm erfolgen.
