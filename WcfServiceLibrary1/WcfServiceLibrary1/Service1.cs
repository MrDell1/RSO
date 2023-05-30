using System;
using System.Collections.Generic;
using System.ServiceModel;
using System.Threading.Tasks;
using System.Collections;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using Google.Apis.Books.v1;
using Google.Apis.Services;

namespace WcfServiceLibrary1
{
    public class LibraryService : WcfServiceLibrary
    {
        private static string API_KEY = "AIzaSyBoibf6O1F8K4xbiEGwsIStoTZ1m6inIyY";

        public static BooksService service = new BooksService(new BaseClientService.Initializer
        {
            ApplicationName = "LibraryWCF",
            ApiKey = API_KEY,
        });
        async public Task<Book> GetBookDetails(string id)
        {
            Console.WriteLine("Executing a book details search request");
            var result = await service.Volumes.List(id).ExecuteAsync();
            if (result != null && result.Items != null)
            {
                var item = result.Items.FirstOrDefault();
                Book book = new Book();
                book.Title = item.VolumeInfo.Title;
                book.Id = item.Id;
                book.Description = item.VolumeInfo.Description;
                book.Authors = (List<string>)item.VolumeInfo.Authors;
                return book;
            }
            var exception = new NoSuchBookException
            {
                Message = "Book not found"
            };
            throw new FaultException<NoSuchBookException>(exception, exception.Message);
        }

        async public Task<List<string>> GetBooks(string query)
        {
            Console.WriteLine("Executing a book id search request");
            var result = await service.Volumes.List(query).ExecuteAsync();
            if (result != null && result.Items != null)
            {
                List<string> books = new List<string>();
                for (int i = 0; i < result.Items.Count; i++)
                {
                    var s = result.Items[i].Id;
                    books.Add(s);
                }
                return books;
            }
            var exception = new NoSuchBookException
            {
                Message = "Book not found"
            };
            throw new FaultException<NoSuchBookException>(exception, exception.Message);
        }
    }
}