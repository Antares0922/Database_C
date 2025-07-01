import pandas as pd
import sqlite3

#SQL
db = sqlite3.connect("Database.db")
cursor = db.cursor()

#Dataframe y CSV
df = pd.read_csv("Datos_crudos/House_Rent_Dataset.csv")

#creando la tabla "Houses"
cursor.execute("""CREATE TABLE IF NOT EXISTS Houses(
               ID_House INTEGER PRIMARY KEY AUTOINCREMENT,
               Posted_On TEXT,
               BHK INTEGER,
               Rent INTEGER,
               Size INTEGER,
               Floor TEXT,
               Area_Type TEXT,
               Area_Locality TEXT,
               City TEXT,
               Furnishing_Status TEXT,
               Tenant_Preferred TEXT,
               Bathroom INTEGER,
               Contact TEXT
)""")

#renombrando el Dataframe
df.rename(columns={
          'Posted On':'Posted_On',
          'Area Type': 'Area_Type',
          'Area Locality': 'Area_Locality',
          'Furnishing Status': 'Furnishing_Status',
          'Tenant Preferred': 'Tenant_Preferred',
          'Point of Contact': 'Contact'},
          inplace=True)

#Agregando el Dataframe a la base de datos
df.to_sql("Houses",db,if_exists='append',index=False)

#Guardando cambios y cerrando
db.commit()
db.close()
