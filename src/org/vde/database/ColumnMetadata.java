package org.vde.database;

public class ColumnMetadata {
  public String name;
  public String dataType;
  public boolean isIndexed;
  public float min = 1000000;
  public float max=-1000000;
  public float pAAWidth = (float) 0.05;
  public float c_min = 1000000;
  public float c_max=-1000000;

}
