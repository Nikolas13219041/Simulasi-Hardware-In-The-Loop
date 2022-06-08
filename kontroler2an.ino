// Source code kontroler untuk simulasi Hardware In The Loop Motor DC
// Nikolas (13219041)

// Set nilai parameter pengendali PID
float Kp = 10;
float Ki = 10;
float Kd = 10;

// Set point yang ingin dicapai
float set_point = 1;

// Error dan control signal
float avg_error, error_1, error_2, ratio_error;
float control_sig = 0;

void setup() 
{
  // Setup kecepatan data 115200 bps
  Serial.begin(9600);
}

void loop() 
{
  Serial.print("I");          // mengirim kode awal FRAME
  Serial.print("S");          // mengirim kode jenis perintah mengambil data
  Serial.print(control_sig);  // mengirim data berupa control signal
  Serial.print("F");          // mengirim kode akhir FRAME

  delay(50);

  Serial.print("I");          // mengirim kode awal FRAME
  Serial.print("T");          // mengirim kode jenis perintah mengirim feedback
  Serial.print("F");          // mengirim kode akhir FRAME
  Serial.flush();             // menunggu sampai semua pesan telah dikirim
  
  // Mengambil data feedback dari plant
  if (Serial.available())
  {
    // deteksi kode awal FRAME
    if (Serial.find('i'))
    {
      // Ambil data feedback dari plant
      float data = Serial.parseFloat();
      
      // deteksi kode akhir FRAME
      if (Serial.read() == 'f')
      {
        // Hitung error
        float error = set_point - data;
        avg_error = (error + error_1)/2;
        ratio_error = (error - 2*error_1 + error_2);
        error_2 = error_1;
        error_1 = error;
        
        // Hitung control signal dari nilai error
        control_sig = set_point + Kp*error + Ki*avg_error + Kd*ratio_error;
      }
    }
  }
}
