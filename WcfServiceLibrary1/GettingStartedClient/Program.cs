using GettingStartedClient.ServiceReference1;
using System;
using System.ServiceModel;


namespace GettingStartedClient
{
    class Program
    {
        static void Main(string[] args)
        {
            WcfServiceLibraryClient proxy = new WcfServiceLibraryClient();

            Console.WriteLine("Client application started.");
            Console.WriteLine(proxy.Test());
            try
            {
                
                while (true)
                {
                    Console.WriteLine("1 - search by id, 2 - search by other things, 3 - exit");
                    var option = Console.ReadLine();
                    var quitter = 0;

                    switch (option)
                    {
                        case "1":
                            Console.WriteLine("Give id:");
                            var query = Console.ReadLine();
                            var book = proxy.GetBookDetails(query);
                            Console.WriteLine("Title: " + book.Title);
                            Console.WriteLine("Desc: " + book.Description);
                            Console.WriteLine("Id: " + book.Id);
                            if (book.Authors != null)
                            {
                                Console.WriteLine("Authors: ");
                                for (int i = 0; i < book.Authors.Length; i++)
                                {
                                    Console.Write(book.Authors[i]);
                                }
                            }
                            Console.WriteLine();
                            
                            break;
                        case "2":
                            Console.WriteLine("Search by query:");
                            var query2 = Console.ReadLine();
                            var books = proxy.GetBooks(query2);
                            for (int i = 0; i < books.Length; i++)
                            {
                                Console.WriteLine("Id: " + books[i].Id);
                                Console.WriteLine("Title: " + books[i].Title);
                                if (book[j].Authors != null)
                                {
                                    Console.WriteLine("Authors: ");
                                    for (int j = 0; j < books[i].Authors.Length; j++)
                                    {
                                        Console.Write(books[i].Authors[j]);
                                    }
                                }
                            }
                            Console.WriteLine();
                            break;
                        case "3":
                            quitter = 1;
                            break;
                        default:
                            Console.WriteLine("Unknown command");
                            break;
                    }
                    if (quitter == 1)
                        break;

                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
