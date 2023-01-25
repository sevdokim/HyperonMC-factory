void addhisto(const TString list="list.txt", char *newname="Out.root")
{
  // Add histograms from a set of files listed in file list
  // and write the result into a new file.

  char fpath[256];
  int ifscanf=0;
  Int_t ifile=0;

  FILE* fd = fopen(list.Data(),"r");
  if(!fd) {
    cout<<"List of files " << list.Data() << " not found!"<<endl;
    return;
  }

  TFileMerger m(kFALSE);

  Bool_t gridConnected=kFALSE;
  while( ifscanf = fscanf(fd,"%s",fpath) != EOF) {
    TString path(fpath);
    if (!gridConnected && path.Contains("alien://")) {
      gridConnected = kTRUE;
      TGrid::Connect("alien://");
    }
    m.AddFile(fpath);

    printf("\n     --- Processing file \"%s\" ---\n",fpath);
    ifile++;
   
  }

  //  m.OutputFile(newname);
  if (newname) m.OutputFile(newname);
  if (ifile)   m.Merge();
  cout << ifile << " processed."<<endl;

}

