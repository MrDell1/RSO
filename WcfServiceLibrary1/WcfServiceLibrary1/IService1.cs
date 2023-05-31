using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Threading.Tasks;

namespace WcfServiceLibrary1
{
        [ServiceContract(Namespace = "http://Microsoft.ServiceModel.Samples")]
        public interface WcfServiceLibrary
    {
        [OperationContract]
        Task<List<Book>> Search(string query);
        [OperationContract]
        Task<List<Book>> GetBooks(string query);

        [OperationContract]
        [FaultContract(typeof(NoSuchBookException))]
        Task<Book> GetBookDetails(string id);
        }
    

        [DataContract]
        public class Book
        {
            [DataMember]
            public string Id { get; set; }
            [DataMember]
            public string Title { get; set; }
            [DataMember]
            public string Description { get; set; }
            [DataMember]
            public List<string> Authors { get; set; }
            [DataMember]
            public string Created { get; set; }
            
        }

        [DataContract]
        public class BookDetails
        {
            [DataMember]
            public string Title { get; set; }
            [DataMember]
            public List<string> Authors { get; set; }
        }

        [DataContract]
        public class NoSuchBookException
        {
            [DataMember]
            public string Message { get; set; }
        }
}