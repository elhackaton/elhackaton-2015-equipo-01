using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;

using Xamarin.Forms;

namespace MapasDeLuz
{
    public class App : Application
    {
        Button b1, b2, b3, b4;

        StackLayout Content;

        public App()
        {
            b1 = new Button
            {
                Text = "Gráficos estadísticos"
            };
            b1.Clicked += OnButtonClicked;
            b2 = new Button
            {
                Text = "Comandos"
            };
            b2.Clicked += OnButtonClicked;
            b3 = new Button
            {
                Text = "Perfiles"
            };
            b3.Clicked += OnButtonClicked;
            b4 = new Button
            {
                Text = "Historicos"
            };
            b4.Clicked += OnButtonClicked;
            // The root page of your application
            MainPage = new ContentPage
            {
                Content = new StackLayout
                {
                    VerticalOptions = LayoutOptions.Center,
                    Children = {
                        b1,
                        b2,
                        b3,
                        b4
                    }
                }
            };
        }

        private void OnButtonClicked(object sender, EventArgs e)
        {
            Button button = (Button)sender;
            if (button == b1)
            {
                //Cambiar vista
                //Leer de la base de datos
                //Generar graficos
            } else if (button == b2)
            {
                WebRequest request = WebRequest.Create("http://hackaton2k15.eu-gb.mybluemix.net/red/gateway/commandexample");
            } else if (button == b3)
            {
                Picker pick = new Picker
                {
                    Title = "Perfil",
                    VerticalOptions = LayoutOptions.CenterAndExpand
                };
                pick.Items.Add("Lectura");
                pick.Items.Add("Descanso");
                pick.Items.Add("Altas exigencias visuales");
                Content.Children.Add(pick);

            } else if (button == b4)
            {
                   //Gestor base de datos
            }
         }

        protected override void OnStart()
        {
            // Handle when your app starts
        }

        protected override void OnSleep()
        {
            // Handle when your app sleeps
        }

        protected override void OnResume()
        {
            // Handle when your app resumes
        }
    }
}
