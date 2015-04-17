import nytquery;

nytquery.init(1,".");
s = nytquery.query(1,10,"video");
print s;
nytquery.close(1);