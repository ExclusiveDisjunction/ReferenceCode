using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Linq;

using Phosphor;
using HmPro.Files;

namespace HmPro.Edit
{
    /// <summary>
    /// This class is in charge of parsing and retriving information from sessions. 
    /// </summary>
    public static class SessionBridge
    {
        public static TreeViewItem GetAllMemes(string Path)
        {
            if (!System.IO.File.Exists(Path))
                Path = Ins.CurrentLoaded;

            IHSession Sess;
            if (Path == Ins.CurrentLoaded)
            {
                Sess = Ins.LoadedSession;
            }
            else
            {
                if (FileTools.FileExt(Path, false) == "teh")
                    Sess = new Session(Path);
                else if (FileTools.FileExt(Path, false) == "tehseg")
                    Sess = new Session(Path);
                else
                    throw new InvalidOperationException("The path specified is not a session for Hmmm Pro.");
            }

            if (Sess is null) return null;

            SolidColorBrush Fore = new SolidColorBrush(Color.FromRgb(0xFF, 0xFF, 0xFF)); // White

            TreeViewItem MasterItem = new TreeViewItem
            {
                Header = Sess.Title,
                Foreground = Fore
            };

            TreeViewItem MemesHeader = new TreeViewItem
            {
                Header = "Memes",
                Foreground = Fore
            };

            IReadOnlySet<Meme> Memes;
            if (Sess is Session) Memes = (Sess as Session).Memes;
            else if (Sess is Segment) Memes = (Sess as Segment).Memes;
            else return null;

            foreach (Meme item in Memes.Sort())
            {
                TreeViewItem Thing = new TreeViewItem
                {
                    Header = item.Title,
                    Foreground = Fore
                };
                MemesHeader.Items.Add(Thing);
            }

            MasterItem.Items.Add(MemesHeader);
            return MasterItem;
        }
        public static TreeViewItem GetAllObjects(string Path)
        {
            return GetObjects(Path, true, true, true, true, true);
        }
        public static TreeViewItem GetAllCollections(string Path, bool ShowChildren)
        {
            return GetObjects(Path, true, false, false, true, false);
        }
        public static TreeViewItem GetAllBins(string Path, bool ShowChildren)
        {
            return GetObjects(Path, false, true, false, false, false);
        }
        public static TreeViewItem GetObjects(string Path, bool ShowMsCollections = true, bool ShowBins = true, bool ShowFLMemes = true, bool ShowCollections = true, bool ShowObjectChildren = true)
        {
            if (!System.IO.File.Exists(Path))
                Path = Ins.CurrentLoaded;

            IHSession Sess;
            if (Path == Ins.CurrentLoaded)
            {
                Sess = Ins.LoadedSession;
            }
            else
            {
                if (FileTools.FileExt(Path, false) == "teh")
                    Sess = new Session(Path);
                else if (FileTools.FileExt(Path, false) == "tehseg")
                    Sess = new Session(Path);
                else
                    throw new InvalidOperationException("The path specified is not a session for Hmmm Pro.");
            }

            if (Sess is null) return null;

            SolidColorBrush Fore = new SolidColorBrush(Color.FromRgb(0xFF, 0xFF, 0xFF)); // White

            TreeViewItem MasterItem = new TreeViewItem
            {
                Header = Sess.Title,
                Foreground = Fore
            };

            if (ShowFLMemes && Sess is Session)
            {
                TreeViewItem FavMemes = new TreeViewItem
                {
                    Header = "Favorite Memes",
                    Foreground = Fore
                };
                TreeViewItem LegMemes = new TreeViewItem
                {
                    Header = "Legendary Memes",
                    Foreground = Fore
                };

                MasterItem.Items.Add(FavMemes);
                MasterItem.Items.Add(LegMemes);

                Session TempFile = Sess as Session;
                foreach (Meme Item in TempFile.FavoriteMemes.Sort())
                {
                    TreeViewItem Temp = new TreeViewItem
                    {
                        Header = Item.Title,
                        Foreground = Fore
                    };
                    FavMemes.Items.Add(Temp);
                }
                foreach (Meme Item in TempFile.LegendaryMemes.Sort())
                {
                    TreeViewItem Temp = new TreeViewItem
                    {
                        Header = Item.Title,
                        Foreground = Fore
                    };
                    LegMemes.Items.Add(Temp);
                }
            }

            if (ShowCollections || ShowMsCollections)
            {
                CollectionList List = Sess is Session ? (Sess as Session).Collections : (Sess as Segment).Collections;
                SortedSet<Collection> Sorted = List.Sort();

                TreeViewItem CollectionMaster = new TreeViewItem
                {
                    Header = "Collections",
                    Foreground = Fore
                };

                MasterItem.Items.Add(CollectionMaster);

                if (ShowCollections && ShowMsCollections)
                {
                    foreach (Collection Coll in Sorted)
                    {
                        TreeViewItem Collection = new TreeViewItem
                        {
                            Header = Coll.Title,
                            Foreground = Fore
                        };

                        CollectionMaster.Items.Add(Collection);

                        if (ShowObjectChildren)
                        {
                            TreeViewItem MemeParent = new TreeViewItem
                            {
                                Header = "Memes",
                                Foreground = Fore
                            };                            

                            Collection.Items.Add(MemeParent);                            

                            foreach (Meme Item in Coll.Memes.Sort())
                            {
                                TreeViewItem item = new TreeViewItem
                                {
                                    Header = Item.Title,
                                    Foreground = Fore
                                };
                                MemeParent.Items.Add(item);
                            }
                        }

                        if (Coll.Type == CollectionTypes.Master)
                        {
                            TreeViewItem CollectionParent = new TreeViewItem
                            {
                                Header = "Collections",
                                Foreground = Fore
                            };
                            Collection.Items.Add(CollectionParent);
                            foreach (Collection Item in Coll.Collections?.Sort())
                            {
                                TreeViewItem item = new TreeViewItem
                                {
                                    Header = Item.Title,
                                    Foreground = Fore
                                };
                                CollectionParent.Items.Add(item);
                            }
                        }
                    }
                }
                else if (ShowCollections && !ShowMsCollections)
                {
                    foreach (Collection Coll in Sorted)
                    {
                        if (Coll.Type == CollectionTypes.Master)
                            continue;

                        TreeViewItem Collection = new TreeViewItem
                        {
                            Header = Coll.Title,
                            Foreground = Fore
                        };

                        CollectionMaster.Items.Add(Collection);

                        if (ShowObjectChildren)
                        {
                            TreeViewItem MemeParent = new TreeViewItem
                            {
                                Header = "Memes",
                                Foreground = Fore
                            };

                            Collection.Items.Add(MemeParent);

                            foreach (Meme item in Coll.Memes.Sort())
                            {
                                TreeViewItem Meme = new TreeViewItem
                                {
                                    Header = item.Title,
                                    Foreground = Fore
                                };
                                MemeParent.Items.Add(Meme);
                            }
                        }
                    }
                }
                else if (ShowMsCollections && !ShowCollections)
                {
                    foreach (Collection Coll in Sorted)
                    {
                        if (Coll.Type != CollectionTypes.Master)
                            continue;

                        TreeViewItem Collection = new TreeViewItem
                        {
                            Header = Coll.Title,
                            Foreground = Fore
                        };

                        CollectionMaster.Items.Add(Collection);

                        if (ShowObjectChildren)
                        {
                            TreeViewItem MemeParent = new TreeViewItem
                            {
                                Header = "Memes",
                                Foreground = Fore
                            };
                            TreeViewItem CollectionParent = new TreeViewItem
                            {
                                Header = "Collections",
                                Foreground = Fore
                            };

                            Collection.Items.Add(MemeParent);
                            Collection.Items.Add(CollectionParent);

                            foreach (Meme Item in Coll.Memes.Sort())
                            {
                                TreeViewItem item = new TreeViewItem
                                {
                                    Header = Item.Title,
                                    Foreground = Fore
                                };
                                MemeParent.Items.Add(item);
                            }
                            foreach (Collection Item in Coll.Collections?.Sort())
                            {
                                TreeViewItem item = new TreeViewItem
                                {
                                    Header = Item.Title,
                                    Foreground = Fore
                                };
                                CollectionParent.Items.Add(item);
                            }
                        }
                    }
                }
            }

            if (ShowBins)
            {
                BinGroup Bins = Sess is Session ? (Sess as Session).Bins : (Sess as Segment).Bins;
                SortedSet<Bin> Sorted = Bins.Sort();

                TreeViewItem BinMaster = new TreeViewItem
                {
                    Header = "Bins",
                    Foreground = Fore
                };

                MasterItem.Items.Add(BinMaster);

                foreach (Bin Bin in Sorted)
                {
                    TreeViewItem BinItem = new TreeViewItem
                    {
                        Header = Bin.Title,
                        Foreground = Fore
                    };

                    BinMaster.Items.Add(BinItem);

                    if (ShowObjectChildren)
                    {
                        TreeViewItem MemesParent = new TreeViewItem
                        {
                            Header = "Memes",
                            Foreground = Fore
                        };
                        TreeViewItem CollectionParent = new TreeViewItem
                        {
                            Header = "Collections",
                            Foreground = Fore
                        };

                        BinItem.Items.Add(MemesParent);
                        BinItem.Items.Add(CollectionParent);

                        foreach (Meme Meme in Bin.Memes?.Sort())
                        {
                            TreeViewItem temp = new TreeViewItem
                            {
                                Header = Meme.Title,
                                Foreground = Fore
                            };
                            MemesParent.Items.Add(temp);
                        }
                        foreach (Collection Collection in Bin.Collections?.Sort())
                        {
                            TreeViewItem temp = new TreeViewItem
                            {
                                Header = Collection.Title,
                                Foreground = Fore
                            };

                            CollectionParent.Items.Add(temp);

                            TreeViewItem SubMemesParent = new TreeViewItem
                            {
                                Header = "Memes",
                                Foreground = Fore
                            };
                            temp.Items.Add(SubMemesParent);

                            foreach (Meme Meme in Collection.Memes.Sort())
                            {
                                TreeViewItem meme = new TreeViewItem
                                {
                                    Header = Meme.Title,
                                    Foreground = Fore
                                };
                                SubMemesParent.Items.Add(meme);
                            }
                        }
                    }
                }
            }

            return MasterItem;
        }
    }
}
