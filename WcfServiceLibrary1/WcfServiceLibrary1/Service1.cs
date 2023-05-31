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

        public String Test()
        {
            return "a";
        }
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
        async public Task<List<Book>> Search(string query)
        {
            Console.WriteLine("Executing a book year search request");
            try
            {
                var years = query.Split('-');
                var year1 = int.Parse(years[0]);
                var year2 = int.Parse(years[1]);
                List<Book> books = new List<Book>();
                for (int i = year1; i <= year2; i++)
                {
                    var result = await service.Volumes.List(query).ExecuteAsync();
                    if (result != null && result.Items != null)
                    {
                        for (int j = 0; j < result.Items.Count; j++)
                        {
                            var s = result.Items[j];
                            Book book = new Book();
                            book.Title = s.VolumeInfo.Title;
                            book.Id = s.Id;
                            book.Authors = (List<string>)s.VolumeInfo.Authors;
                            book.Created = s.VolumeInfo.PublishedDate;
                            books.Add(book);
                        }
                    }
                    else
                    {
                        var exception = new NoSuchBookException
                        {
                            Message = "Book not found"
                        };
                        throw new FaultException<NoSuchBookException>(exception, exception.Message);
                    }
                }
                return books;
            }
            catch(Exception ex)
            {
                throw new Exception();

            }
        }
        async public Task<List<Book>> GetBooks(string query)
        {
            Console.WriteLine("Executing a book id search request");
            var result = await service.Volumes.List(query).ExecuteAsync();
            if (result != null && result.Items != null)
            {
                List<Book> books = new List<Book>();
                for (int i = 0; i < result.Items.Count; i++)
                {
                    var s = result.Items[i];
                    Book book = new Book();
                    book.Title = s.VolumeInfo.Title;
                    book.Id = s.Id;
                    book.Authors = (List<string>)s.VolumeInfo.Authors;
                    book.Created = s.VolumeInfo.PublishedDate;
                    books.Add(book);
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