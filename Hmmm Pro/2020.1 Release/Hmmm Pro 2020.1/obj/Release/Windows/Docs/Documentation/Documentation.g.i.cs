﻿#pragma checksum "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml" "{8829d00f-11b8-4213-878b-770e8597ac16}" "941B909E1CF868328DECD9A5CD03C58D1A33A9D6B76778DB4F2BA684D9DDEC71"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using HmPro.Windows.Docs;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace HmPro.Windows.Docs {
    
    
    /// <summary>
    /// Documentation
    /// </summary>
    public partial class Documentation : System.Windows.Window, System.Windows.Markup.IComponentConnector {
        
        
        #line 54 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBlock itTitle;
        
        #line default
        #line hidden
        
        
        #line 55 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox itText;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/Hmmm Pro;component/windows/docs/documentation/documentation.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            
            #line 8 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((HmPro.Windows.Docs.Documentation)(target)).Closing += new System.ComponentModel.CancelEventHandler(this.Window_Closing);
            
            #line default
            #line hidden
            
            #line 8 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((HmPro.Windows.Docs.Documentation)(target)).Loaded += new System.Windows.RoutedEventHandler(this.Window_Loaded);
            
            #line default
            #line hidden
            return;
            case 2:
            
            #line 23 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.Intro);
            
            #line default
            #line hidden
            return;
            case 3:
            
            #line 24 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.EditorsAndActions);
            
            #line default
            #line hidden
            return;
            case 4:
            
            #line 25 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.AddCollection);
            
            #line default
            #line hidden
            return;
            case 5:
            
            #line 26 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.AddMeme);
            
            #line default
            #line hidden
            return;
            case 6:
            
            #line 27 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.AllObjects);
            
            #line default
            #line hidden
            return;
            case 7:
            
            #line 28 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.BasicEditor);
            
            #line default
            #line hidden
            return;
            case 8:
            
            #line 29 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.CurrentFile);
            
            #line default
            #line hidden
            return;
            case 9:
            
            #line 30 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.ManageCollections);
            
            #line default
            #line hidden
            return;
            case 10:
            
            #line 31 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.ManageMemes);
            
            #line default
            #line hidden
            return;
            case 11:
            
            #line 32 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.Settings);
            
            #line default
            #line hidden
            return;
            case 12:
            
            #line 35 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.File);
            
            #line default
            #line hidden
            return;
            case 13:
            
            #line 36 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.Memes);
            
            #line default
            #line hidden
            return;
            case 14:
            
            #line 37 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.Edit);
            
            #line default
            #line hidden
            return;
            case 15:
            
            #line 38 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.Window);
            
            #line default
            #line hidden
            return;
            case 16:
            
            #line 39 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.Help);
            
            #line default
            #line hidden
            return;
            case 17:
            
            #line 43 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.AppendAndAppendAll_Selected);
            
            #line default
            #line hidden
            return;
            case 18:
            
            #line 44 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.SaveAndSaveAll_Selected);
            
            #line default
            #line hidden
            return;
            case 19:
            
            #line 45 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.Importing_Selected);
            
            #line default
            #line hidden
            return;
            case 20:
            
            #line 46 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.Exporting_Selected);
            
            #line default
            #line hidden
            return;
            case 21:
            
            #line 49 "..\..\..\..\..\Windows\Docs\Documentation\Documentation.xaml"
            ((System.Windows.Controls.TreeViewItem)(target)).Selected += new System.Windows.RoutedEventHandler(this.KeyboardShortcuts);
            
            #line default
            #line hidden
            return;
            case 22:
            this.itTitle = ((System.Windows.Controls.TextBlock)(target));
            return;
            case 23:
            this.itText = ((System.Windows.Controls.TextBox)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}
