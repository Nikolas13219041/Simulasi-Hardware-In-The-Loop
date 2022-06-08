// Source code plant untuk simulasi Hardware In The Loop Motor DC
// Nikolas (13219041)

// Deklarasi variabel untuk transfer function dari filter digital
float y, yn1, xn, xn1;

void setup() 
{
  // Setup kecepatan data 115200 bps
  Serial.begin(9600);
  
  //inisialisasi agar bisa diakses oleh program PLX-DAQ yg menghasilkan file CSV
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,Signal Control,Signal Output");
  Serial.println("RESETTIMER");
}

void loop() 
{
  if(Serial.available()>0)
  {
    if(Serial.read()=='I')                     // deteksi kode awal FRAME
    {
      char kodePerintah = Serial.read();       // membaca kode jenis perintah
      
      // Jika kode perintah S, plant mengambil data dari kontroler
      if(kodePerintah=='S'){
        xn = Serial.parseFloat();
        if(Serial.read() == 'F'){              // deteksi kode akhir FRAME
          y = 0.002237*xn + 0.002237*xn1 + 0.995526*yn1;
          yn1 = y;
          xn1 = xn;
        }
      }
      
      // Jika kode perintah T, plant mengirim feedback ke kontroler
      else if(kodePerintah == 'T'){
        if (Serial.read() == 'F'){            // deteksi kode akhir FRAME
          // Kirim feedback dengan format: i, data, f
          Serial.print("i");
          Serial.print(y);
          Serial.println("f");
          
          // Proses logging data ke excel dengan PLX-DAQ
          Serial.print("DATA");
          Serial.print(",");
          Serial.print("TIME");
          Serial.print(",");
          Serial.print(xn); 
          Serial.print(","); 
          Serial.println(y);
        }
      }
    }
  }
  delay(100);
}
